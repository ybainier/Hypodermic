#pragma once

#include <mutex>
#include <unordered_map>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationActivationInterceptor.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class PersistentInstanceRegistrationActivator : public IRegistrationActivator,
                                                    public IRegistrationActivationInterceptor
    {
    public:
        explicit PersistentInstanceRegistrationActivator(const IRegistration& registration)
            : m_registration(registration)
        {
        }

        std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            return activate(*this, container, typeAliasKey);
        }

        std::shared_ptr< void > activate(IRegistrationActivationInterceptor&, Container& container, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating persistent instance of type " << m_registration.instanceType().fullyQualifiedName());

            {
                std::lock_guard< decltype(m_mutex) > lock(m_mutex);

                auto instanceIt = m_activatedInstances.find(typeAliasKey);
                if (instanceIt != std::end(m_activatedInstances))
                    return instanceIt->second;

                if (m_instance != nullptr)
                {
                    auto instance = RegistrationExtensions::getAlignedPointer(m_registration, m_instance, typeAliasKey);
                    m_activatedInstances.insert(std::make_pair(typeAliasKey, instance));

                    return instance;
                }
            }

            return m_registration.activator().activate(*this, container, typeAliasKey);
        }

        void onSourceRegistrationActivated(const std::shared_ptr< void >& instance) override
        {
            if (instance == nullptr)
                return;

            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            m_instance = instance;
        }

        void onRegistrationActivated(const std::shared_ptr< void >& instance, const TypeAliasKey& typeAliasKey) override
        {
            if (instance == nullptr)
                return;

            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            m_activatedInstances.insert(std::make_pair(typeAliasKey, instance));
        }

    private:
        const IRegistration& m_registration;
        std::unordered_map< TypeAliasKey, std::shared_ptr< void > > m_activatedInstances;
        std::recursive_mutex m_mutex;
        std::shared_ptr< void > m_instance;
    };

} // namespace Hypodermic