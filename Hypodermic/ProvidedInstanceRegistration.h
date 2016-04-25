#pragma once

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    template <class T>
    class ProvidedInstanceRegistration : public IRegistration
    {
    public:
        ProvidedInstanceRegistration(const std::shared_ptr< T >& instance,
                                     const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases)
            : m_instance(instance)
            , m_instanceType(Utils::getMetaTypeInfo< T >())
            , m_typeAliases(typeAliases)
        {
        }
    
        const TypeInfo& instanceType() const override
        {
            return m_instanceType;
        }

        const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases() const override
        {
            return m_typeAliases;
        }

        std::function< std::shared_ptr< void >(Container&) > getDependencyFactory(const TypeInfo&) const override
        {
            return nullptr;
        }

        std::shared_ptr< void > activate(Container&, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating provided instance of type " << m_instanceType.fullyQualifiedName());

            std::shared_ptr< void > instance = m_instance;

            auto it = m_typeAliases.find(typeAliasKey);
            if (it != std::end(m_typeAliases) && it->second != nullptr)
            {
                auto&& alignPointersFunc = it->second;
                instance = alignPointersFunc(instance);
            }

            if (instance == nullptr)
                HYPODERMIC_LOG_WARN("Provided instance of type " << m_instanceType.fullyQualifiedName() << " is null");

            return instance;
        }

    private:
        std::shared_ptr< T > m_instance;
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
    };

} // namespace Hypodermic