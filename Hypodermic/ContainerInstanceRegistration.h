#pragma once

#include "Hypodermic/Container.h"
#include "Hypodermic/ContainerInstanceRegistrationActivator.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class ContainerInstanceRegistration : public IRegistration
    {
    public:
        explicit ContainerInstanceRegistration(const std::shared_ptr< Container >& instance)
            : m_activator(*this, instance)
            , m_instanceType(Utils::getMetaTypeInfo< Container >())
            , m_typeAliases()
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
        mutable ContainerInstanceRegistrationActivator m_activator;
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
    };

} // namespace Hypodermic