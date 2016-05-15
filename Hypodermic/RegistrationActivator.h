#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <boost/signals2.hpp>

#include "Hypodermic/CircularDependencyException.h"
#include "Hypodermic/DependencyActivationException.h"
#include "Hypodermic/InstanceAlreadyActivatingException.h"
#include "Hypodermic/InvokeAtScopeExit.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/NoopRegistrationActivationInterceptor.h"
#include "Hypodermic/RegistrationExtensions.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class RegistrationActivator : public IRegistrationActivator,
                                  public NoopRegistrationActivationInterceptor
    {
    public:
        RegistrationActivator(const IRegistration& registration,
                              const std::function< std::shared_ptr< void >(Container&) >& instanceFactory,
                              const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
            : m_registration(registration)
            , m_instanceFactory(instanceFactory)
            , m_activating(false)
        {
            for (auto&& handler : activationHandlers)
                m_activated.connect(handler);
        }

         std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            return activate(*this, container, typeAliasKey);
        }

        std::shared_ptr< void > activate(IRegistrationActivationInterceptor& activationInterceptor, Container& container, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating type " << m_registration.instanceType().fullyQualifiedName());

            Utils::InvokeAtScopeExit atScopeExit([this]() { this->m_activating = false; });

            auto&& typeInfo = typeAliasKey.typeAlias().typeInfo();

            {
                std::lock_guard< decltype(m_mutex) > lock(m_mutex);

                if (m_activating)
                {
                    if (m_registration.instanceType() == typeInfo)
                    {
                        HYPODERMIC_LOG_ERROR("Already activating '" << typeInfo.fullyQualifiedName() << "', unwinding...");

                        HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION("Already activating '" << typeInfo.fullyQualifiedName() << "', unwinding...");
                    }
                    else
                    {
                        HYPODERMIC_LOG_ERROR("Already activating '" << typeInfo.fullyQualifiedName() << "' (base of '" <<
                                             m_registration.instanceType().fullyQualifiedName() << "'), unwinding...");

                        HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION("Already activating '" << typeInfo.fullyQualifiedName() << "' (base of '" <<
                                                                               m_registration.instanceType().fullyQualifiedName() << "'), unwinding...");
                    }
                }

                m_activating = true;
            }

            if (!m_instanceFactory)
            {
                HYPODERMIC_LOG_WARN("No instance factory provided to activate type " << m_registration.instanceType().fullyQualifiedName());
                return nullptr;
            }

            try
            {
                auto&& instance = m_instanceFactory(container);
                activationInterceptor.onSourceRegistrationActivated(instance);

                instance = RegistrationExtensions::getAlignedPointer(m_registration, instance, typeAliasKey);

                activationInterceptor.onRegistrationActivated(instance, typeAliasKey);

                if (instance == nullptr)
                {
                    HYPODERMIC_LOG_WARN("Instance of type " << m_registration.instanceType().fullyQualifiedName() << " is null");
                }
                else
                {
                    m_activated(container, instance);
                }

                return instance;
            }
            catch (CircularDependencyException& ex)
            {
                HYPODERMIC_LOG_ERROR("Circular dependency detected while activating type " << m_registration.instanceType().fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION("'" << m_registration.instanceType().fullyQualifiedName() << "' -> " << ex.what());
            }
            catch (InstanceAlreadyActivatingException& ex)
            {
                HYPODERMIC_LOG_ERROR("Circular dependency detected while activating type " << m_registration.instanceType().fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION("'" << m_registration.instanceType().fullyQualifiedName() << "': " << ex.what());
            }
            catch (DependencyActivationException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << m_registration.instanceType().fullyQualifiedName() <<
                                     " because one dependency cannot be activated: " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << m_registration.instanceType().fullyQualifiedName() <<
                                                                 " because one dependency cannot be activated: " << ex.what());
            }
            catch (std::exception& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << m_registration.instanceType().fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << m_registration.instanceType().fullyQualifiedName() <<
                                                                 ": " << ex.what());
            }
        }

        void onSourceRegistrationActivated(const std::shared_ptr< void >&) override
        {
        }

        void onRegistrationActivated(const std::shared_ptr< void >&, const TypeAliasKey&) override
        {
        }

    private:
        const IRegistration& m_registration;
        std::function< std::shared_ptr< void >(Container&) > m_instanceFactory;
        bool m_activating;
        std::recursive_mutex m_mutex;
        boost::signals2::signal< void(Container&, const std::shared_ptr< void >&) > m_activated;
    };

} // namespace Hypodermic