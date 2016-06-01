#pragma once

#include <exception>
#include <mutex>
#include <sstream>

#include "Hypodermic/ActivationResult.h"
#include "Hypodermic/ActivationException.h"
#include "Hypodermic/ActivatedRegistrationInfo.h"
#include "Hypodermic/ActivationRegistry.h"
#include "Hypodermic/CircularDependencyException.h"
#include "Hypodermic/DependencyActivationException.h"
#include "Hypodermic/InstanceAlreadyActivatingException.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/IResolutionContainer.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/PointerAlignment.h"
#include "Hypodermic/ResolutionContext.h"


namespace Hypodermic
{
    
    class ResolutionContainer : public IResolutionContainer
    {
    public:
        std::shared_ptr< void > getOrCreateComponent(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration, ResolutionContext& resolutionContext) override
        {
            auto& resolutionStack = resolutionContext.resolutionStack();
            auto& activatedRegistrations = resolutionContext.activatedRegistrations();

            checkForCircularDependencies(typeAliasKey, registration, resolutionContext);

            resolutionStack.push_back(ResolutionInfo(registration, typeAliasKey));

            ActivationResult activationResult;

            try
            {
                activationResult = getOrActivateComponent(typeAliasKey, registration, resolutionContext);
            }
            catch (CircularDependencyException& ex)
            {
                HYPODERMIC_LOG_ERROR("Circular dependency detected while activating " << registration->instanceType().fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION("Circular dependency detected while activating " << registration->instanceType().fullyQualifiedName() << ": " << ex.what());
            }
            catch (InstanceAlreadyActivatingException& ex)
            {
                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION(ex.what());
            }
            catch (DependencyActivationException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << registration->instanceType().fullyQualifiedName() << " because one dependency cannot be activated: " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << registration->instanceType().fullyQualifiedName() <<
                                                                 " because one dependency cannot be activated: " << ex.what());
            }
            catch (ActivationException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << registration->instanceType().fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << registration->instanceType().fullyQualifiedName() << ": " << ex.what());
            }
            catch (std::exception& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << registration->instanceType().fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_ACTIVATION_EXCEPTION("Unable to activate instance of type " << registration->instanceType().fullyQualifiedName() << ": " << ex.what());
            }

            resolutionStack.pop_back();

            if (activationResult.activated)
                activatedRegistrations.push_back(ActivatedRegistrationInfo(registration, activationResult.activatedInstance));

            if (resolutionStack.empty())
                notifyActivatedRegistrations(activatedRegistrations, resolutionContext.componentContext());

            return activationResult.alignedInstance;
        }

    private:
        ActivationResult getOrActivateComponent(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration, IResolutionContext& resolutionContext)
        {
            if (registration->instanceLifetime() == InstanceLifetimes::Transient)
                return activateComponent(typeAliasKey, registration, resolutionContext);

            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto registryIt = m_activationRegistriesByRegistration.find(registration);
            if (registryIt == std::end(m_activationRegistriesByRegistration))
                return activateComponentAndRegisterActivatedInstance(typeAliasKey, registration, resolutionContext);

            auto&& registry = registryIt->second;

            ActivationResult activationResult;
            activationResult.activated = false;
            activationResult.activatedInstance = registry.activatedInstance();
            activationResult.alignedInstance = registry.getOrCreateAlignedInstance(typeAliasKey, registration->typeAliases());

            return activationResult;
        }

        ActivationResult activateComponentAndRegisterActivatedInstance(const TypeAliasKey& typeAliasKey,
                                                                       const std::shared_ptr< IRegistration >& registration,
                                                                       IResolutionContext& resolutionContext)
        {
            auto activationResult = activateComponent(typeAliasKey, registration, resolutionContext);

            if (activationResult.activated)
                m_activationRegistriesByRegistration.insert(std::make_pair(registration, ActivationRegistry(activationResult.activatedInstance)));

            return activationResult;
        }

        static ActivationResult activateComponent(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration, IResolutionContext& resolutionContext)
        {
            ActivationResult activationResult;
            activationResult.activatedInstance = activateInstance(registration, resolutionContext);
            activationResult.alignedInstance = Utils::getAlignedPointer(activationResult.activatedInstance, typeAliasKey, registration->typeAliases());
            activationResult.activated = activationResult.activatedInstance != nullptr;

            return activationResult;
        }

        static std::shared_ptr< void > activateInstance(const std::shared_ptr< IRegistration >& registration, IResolutionContext& resolutionContext)
        {
            auto&& instance = registration->activator().activate(resolutionContext);
            if (instance == nullptr)
            {
                HYPODERMIC_LOG_WARN("Activated instance of type " << registration->instanceType().fullyQualifiedName() << " is null");

                return nullptr;
            }

            return instance;
        }

        static void notifyActivatedRegistrations(ResolutionContext::ActivatedRegistrations& activatedRegistrations, ComponentContext& componentContext)
        {
            auto infos = activatedRegistrations;
            activatedRegistrations.clear();

            for (auto&& info : infos)
                info.registration()->activator().raiseActivated(componentContext, info.instance());
        }

        static void checkForCircularDependencies(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration, ResolutionContext& resolutionContext)
        {
            if (!isActivating(registration, resolutionContext.resolutionStack()))
                return;

            std::stringstream stream;
            stream << "Already activating " << prettyPrintResolutionInfo(*registration, typeAliasKey) << ": ";

            auto firstItem = true;
            for (auto&& resolutionInfo : resolutionContext.resolutionStack())
            {
                if (firstItem)
                    firstItem = false;
                else
                    stream << " -> ";

                stream << prettyPrintResolutionInfo(*resolutionInfo.registration(), resolutionInfo.typeAliasKey());
            }

            auto errorMessage = stream.str();
            HYPODERMIC_LOG_ERROR(errorMessage);
            HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION(errorMessage);
        }

        static std::string prettyPrintResolutionInfo(const IRegistration& registration, const TypeAliasKey& typeAliasKey)
        {
            std::stringstream stream;

            if (registration.instanceType() == typeAliasKey.typeAlias().typeInfo())
                stream << typeAliasKey.typeAlias().typeInfo().fullyQualifiedName();
            else
                stream << typeAliasKey.typeAlias().typeInfo().fullyQualifiedName() << " (base of " << registration.instanceType().fullyQualifiedName() << ")";

            return stream.str();
        } 

        static bool isActivating(const std::shared_ptr< IRegistration >& registration, const ResolutionContext::ResolutionStack& resolutionStack)
        {
            for (auto&& resolutionInfo : resolutionStack)
            {
                if (resolutionInfo.registration() == registration)
                    return true;
            }

            return false;
        }

    private:
        std::unordered_map< std::shared_ptr< IRegistration >, ActivationRegistry > m_activationRegistriesByRegistration;
        std::recursive_mutex m_mutex;
    };

} // namespace Hypodermic