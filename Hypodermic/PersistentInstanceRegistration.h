#pragma once

#include <mutex>
#include <unordered_map>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class PersistentInstanceRegistration : public IRegistration
    {
    public:
        explicit PersistentInstanceRegistration(const std::shared_ptr< IRegistration >& registration)
            : m_registration(registration)
        {
        }

        const TypeInfo& instanceType() const override
        {
            return m_registration->instanceType();
        }

        const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases() const override
        {
            return m_registration->typeAliases();
        }

        std::function< std::shared_ptr< void >(Container&) > getDependencyFactory(const TypeInfo& dependencyType) const override
        {
            return m_registration->getDependencyFactory(dependencyType);
        }

        std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating persistent instance of type " << instanceType().fullyQualifiedName());

            {
                std::lock_guard< decltype(m_mutex) > lock(m_mutex);

                auto instanceIt = m_activatedInstances.find(typeAliasKey);
                if (instanceIt != std::end(m_activatedInstances))
                    return instanceIt->second;
            }

            auto instance = m_registration->activate(container, typeAliasKey);
            if (instance != nullptr)
            {
                std::lock_guard< decltype(m_mutex) > lock(m_mutex);

                m_activatedInstances.insert(std::make_pair(typeAliasKey, instance));
            }

            return instance;
        }

    private:
        std::shared_ptr< IRegistration > m_registration;
        std::unordered_map< TypeAliasKey, std::shared_ptr< void > > m_activatedInstances;
        std::recursive_mutex m_mutex;
    };

} // namespace Hypodermic