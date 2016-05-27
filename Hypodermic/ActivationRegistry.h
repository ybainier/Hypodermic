#pragma once

#include <memory>
#include <unordered_map>

#include "Hypodermic/PointerAlignment.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{
    
    class ActivationRegistry
    {
    public:
        explicit ActivationRegistry(const std::shared_ptr< void >& activatedInstance)
            : m_activatedInstance(activatedInstance)
        {
        }

        const std::shared_ptr< void >& activatedInstance() const
        {
            return m_activatedInstance;
        }

        std::shared_ptr< void > getOrCreateAlignedInstance(const TypeAliasKey& typeAliasKey, const TypeAliases& typeAliases)
        {
            auto it = m_alignedInstances.find(typeAliasKey);
            if (it == std::end(m_alignedInstances))
                it = m_alignedInstances.insert(std::make_pair(typeAliasKey, Utils::getAlignedPointer(m_activatedInstance, typeAliasKey, typeAliases))).first;

            return it->second;
        }
        
    private:
        std::shared_ptr< void > m_activatedInstance;
        std::unordered_map< TypeAliasKey, std::shared_ptr< void > > m_alignedInstances;
    };

} // namespace