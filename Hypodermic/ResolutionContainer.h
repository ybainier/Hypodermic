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
#include "Hypodermic/ResolutionContext.h"


namespace Hypodermic
{
    
    class ResolutionContainer : public IResolutionContainer
    {
    public:
        std::shared_ptr< void > getOrCreateComponent(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration, ResolutionContext& resolutionContext) override
        {
            auto& componentContext = resolutionContext.componentContext();
            auto& resolutionStack = resolutionContext.resolutionStack();
            auto& activatedRegistrations = resolutionContext.activatedRegistrations();

            checkForCircularDependencies(typeAliasKey, registration, resolutionContext);

            resolutionStack.push_back(ResolutionInfo(registration, typeAliasKey));

            ActivationResult activationResult;

            try
            {
                activationResult = getOrCreateComponent(componentContext, typeAliasKey, registration);
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

            if (activationResult.activated && activationResult.sourceInstance != nullptr)
                activatedRegistrations.push_back(ActivatedRegistrationInfo(registration, activationResult.sourceInstance));

            if (resolutionStack.empty())
                notifyActivatedRegistrations(activatedRegistrations, componentContext);

            return activationResult.alignedInstance;
        }

    private:
        ActivationResult getOrCreateComponent(ComponentContext& componentContext, const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration)
        {
            ActivationResult instanceInfo;
            instanceInfo.activated = true;

            if (registration->instanceLifetime() == InstanceLifetimes::Transient)
            {
                instanceInfo.sourceInstance = activateInstance(componentContext, registration);
                instanceInfo.alignedInstance = getAlignedPointer(*registration, instanceInfo.sourceInstance, typeAliasKey);
                return instanceInfo;
            }

            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto registryIt = m_activationRegistriesByRegistration.find(registration);
            if (registryIt == std::end(m_activationRegistriesByRegistration))
            {
                instanceInfo.sourceInstance = activateInstance(componentContext, registration);
                if (instanceInfo.sourceInstance == nullptr)
                    return instanceInfo;

                registryIt = m_activationRegistriesByRegistration.insert(std::make_pair(registration, ActivationRegistry(instanceInfo.sourceInstance))).first;
            }
            else
            {
                instanceInfo.activated = false;
                instanceInfo.sourceInstance = registryIt->second.sourceInstance();
            }

            auto&& registry = registryIt->second;

            if (registry.tryGetInstance(typeAliasKey, instanceInfo.alignedInstance))
                return instanceInfo;

            instanceInfo.alignedInstance = getAlignedPointer(*registration, instanceInfo.sourceInstance, typeAliasKey);
            registry.addInstance(typeAliasKey, instanceInfo.alignedInstance);

            return instanceInfo;
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