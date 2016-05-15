#pragma once

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/ProvidedInstanceRegistrationActivator.h"
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
            : m_activator(*this, instance)
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

        IRegistrationActivator& activator() const override
        {
            return m_activator;
        }

    private:
        mutable ProvidedInstanceRegistrationActivator< T > m_activator;
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
    };

} // namespace Hypodermic