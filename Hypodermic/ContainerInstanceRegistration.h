#pragma once

#include "Hypodermic/Container.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/NoopRegistrationActivationInterceptor.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class ContainerInstanceRegistration : public IRegistration,
                                          public IRegistrationActivator,
                                          public NoopRegistrationActivationInterceptor
    {
    public:
        explicit ContainerInstanceRegistration(const std::shared_ptr< Container >& instance)
            : m_instance(instance)
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

        IRegistrationActivator& activator() override
        {
            return *this;
        }

        std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            return activate(*this, container, typeAliasKey);
        }

        std::shared_ptr< void > activate(IRegistrationActivationInterceptor&, Container&, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating Container instance of type " << m_instanceType.fullyQualifiedName());

            std::shared_ptr< void > instance = m_instance.lock();
            if (instance == nullptr)
                HYPODERMIC_LOG_WARN("Container instance of type " << m_instanceType.fullyQualifiedName() << " is null");

            return instance;
        }

    private:
        std::weak_ptr< Container > m_instance;
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
    };

} // namespace Hypodermic