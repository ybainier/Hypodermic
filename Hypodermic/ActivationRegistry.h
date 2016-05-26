#pragma once

#include <memory>
#include <unordered_map>

#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{
    
    class ActivationRegistry
    {
    public:
        explicit ActivationRegistry(const std::shared_ptr< void >& sourceInstance)
            : m_sourceInstance(sourceInstance)
        {
        }

        const std::shared_ptr< void >& sourceInstance() const
        {
            return m_sourceInstance;
        }

        bool tryGetInstance(const TypeAliasKey& typeAliasKey, std::shared_ptr< void >& instance) const
        {
            auto it = m_activatedInstances.find(typeAliasKey);
            if (it == std::end(m_activatedInstances))
                return false;

            instance = it->second;
            return true;
        }

        void addInstance(const TypeAliasKey& typeAliasKey, const std::shared_ptr< void >& instance)
        {
            m_activatedInstances[typeAliasKey] = instance;
        }
        
    private:
        std::shared_ptr< void > m_sourceInstance;
        std::unordered_map< TypeAliasKey, std::shared_ptr< void > > m_activatedInstances;
    };

} // namespace