#pragma once

#include <exception>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "Hypodermic/ActivationException.h"
#include "Hypodermic/ActivationRegistry.h"
#include "Hypodermic/CircularDependencyException.h"
#include "Hypodermic/DependencyActivationException.h"
#include "Hypodermic/InstanceAlreadyActivatingException.h"
#include "Hypodermic/InvokeAtScopeExit.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/IResolutionScope.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/RegistrationContext.h"
#include "Hypodermic/ResolutionContext.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{

    class RegistrationScope : public IRegistrationScope, public IResolutionScope
    {
    public:
        void addRegistration(const std::shared_ptr< IRegistration >& registration) override
        {
            if (registration->typeAliases().empty())
            {
                addRegistration(createKeyForType(registration->instanceType()), registration);
                return;
            }

            for (auto&& x : registration->typeAliases())
                addRegistration(x.first, registration);
        }
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const override
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto it = m_registrationContextsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationContextsByBaseTypes))
                return false;

            registrationContexts = it->second;
            return true;
        }

        std::shared_ptr< void > getOrCreateComponent(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration, ResolutionContext& resolutionContext) override
        {
            auto& componentContext = resolutionContext.componentContext();
            auto& activationStack = resolutionContext.activationStack();
            auto& activatedRegistrations = resolutionContext.activatedRegistrations();

            auto& instanceType = registration->instanceType();

            bool activated;
            std::shared_ptr< void > instance;

            Utils::InvokeAtScopeFailure atScopeFailure([&activationStack]() { activationStack.clear(); });

            if (isActivating(registration, activationStack))
            {
                auto&& typeInfo = typeAliasKey.typeAlias().typeInfo();

                if (instanceType == typeInfo)
                {
                    HYPODERMIC_LOG_ERROR("Already activating " << typeInfo.fullyQualifiedName() << ", unwinding...");

                    HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION("Already activating " << typeInfo.fullyQualifiedName() << ", unwinding...");
                }

                HYPODERMIC_LOG_ERROR("Already activating " << typeInfo.fullyQualifiedName() << " (base of " << instanceType.fullyQualifiedName() << "), unwinding...");

                HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION("Already activating " << typeInfo.fullyQualifiedName() << " (base of " <<
                                                                       instanceType.fullyQualifiedName() << "), unwinding...");
            }

            activationStack.push_back(registration);

            try
            {
                instance = getOrCreateComponent(componentContext, typeAliasKey, registration, activated);
            }
            catch (CircularDependencyException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << instanceType.fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION(instanceType.fullyQualifiedName() << " -> " << ex.what());
            }
            catch (InstanceAlreadyActivatingException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << instanceType.fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION(instanceType.fullyQualifiedName() << ": " << ex.what());
            }
            catch (DependencyActivationException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << instanceType.fullyQualifiedName() << " because one dependency cannot be activated: " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << instanceType.fullyQualifiedName() <<
                                                                 " because one dependency cannot be activated: " << ex.what());
            }
            catch (ActivationException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << instanceType.fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << instanceType.fullyQualifiedName() << ": " << ex.what());
            }
            catch (std::exception& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << instanceType.fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_ACTIVATION_EXCEPTION("Unable to activate instance of type " << instanceType.fullyQualifiedName() << ": " << ex.what());
            }

            activationStack.pop_back();

            if (activated && instance != nullptr)
                activatedRegistrations.push_back(registration);

            if (activationStack.empty())
                notifyActivatedRegistrations(activatedRegistrations, componentContext, instance);

            return instance;
        }

    private:
        static void notifyActivatedRegistrations(ActivationStack& activatedRegistrations, ComponentContext& componentContext, const std::shared_ptr< void >& instance)
        {
            auto registrations = activatedRegistrations;
            activatedRegistrations.clear();

            for (auto&& registration : registrations)
                registration->activator().raiseActivated(componentContext, instance);
        }

        static bool isActivating(const std::shared_ptr< IRegistration >& registration, const ActivationStack& activationStack)
        {
            return std::find(std::begin(activationStack), std::end(activationStack), registration) != std::end(activationStack);
        }

        std::shared_ptr< void > getOrCreateComponent(ComponentContext& componentContext, const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration, bool& activated)
        {
            activated = true;

            if (registration->instanceLifetime() == InstanceLifetimes::Transient)
                return getAlignedPointer(*registration, activateInstance(componentContext, registration), typeAliasKey);

            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto registryIt = m_activationRegistriesByRegistration.find(registration);
            if (registryIt == std::end(m_activationRegistriesByRegistration))
            {
                auto&& instance = activateInstance(componentContext, registration);
                if (instance == nullptr)
                    return nullptr;

                registryIt = m_activationRegistriesByRegistration.insert(std::make_pair(registration, ActivationRegistry(instance))).first;
            }
            else
            {
                activated = false;
            }

            auto&& registry = registryIt->second;

            std::shared_ptr< void > instance;
            if (registry.tryGetInstance(typeAliasKey, instance))
                return instance;

            instance = getAlignedPointer(*registration, registry.sourceInstance(), typeAliasKey);
            registry.addInstance(typeAliasKey, instance);

            return instance;
        }

        void addRegistration(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration)
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto it = m_registrationContextsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationContextsByBaseTypes))
                it = m_registrationContextsByBaseTypes.insert(std::make_pair(typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >())).first;

            it->second.push_back(std::make_shared< RegistrationContext >(*this, registration));
        }

        static std::shared_ptr< void > getAlignedPointer(const IRegistration& registration, const std::shared_ptr< void >& instance, const TypeAliasKey& typeAliasKey)
        {
            if (instance == nullptr)
                return nullptr;

            auto it = registration.typeAliases().find(typeAliasKey);
            if (it == std::end(registration.typeAliases()) || it->second == nullptr)
                return instance;

            auto&& alignPointersFunc = it->second;
            return alignPointersFunc(instance);
        }

        static std::shared_ptr< void > activateInstance(ComponentContext& componentContext, const std::shared_ptr< IRegistration >& registration)
        {
            auto&& instance = registration->activator().activate(componentContext);
            if (instance == nullptr)
            {
                HYPODERMIC_LOG_WARN("Activated instance of type " << registration->instanceType().fullyQualifiedName() << " is null");

                return nullptr;
            }

            return instance;
        }

    private:
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< RegistrationContext > > > m_registrationContextsByBaseTypes;
        mutable std::recursive_mutex m_mutex;
        std::unordered_map< std::shared_ptr< IRegistration >, ActivationRegistry > m_activationRegistriesByRegistration;
    };

} // namespace Hypodermic