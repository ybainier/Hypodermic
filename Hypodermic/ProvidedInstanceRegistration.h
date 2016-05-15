#pragma once

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/NoopRegistrationActivationInterceptor.h"
#include "Hypodermic/RegistrationExtensions.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    template <class T>
    class ProvidedInstanceRegistration : public IRegistration,
                                         public IRegistrationActivator,
                                         public NoopRegistrationActivationInterceptor
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

        IRegistrationActivator& activator() override
        {
            return *this;
        }

        std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            return activate(*this, container, typeAliasKey);
        }

        std::shared_ptr< void > activate(IRegistrationActivationInterceptor& activationInterceptor, Container&, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating provided instance of type " << m_instanceType.fullyQualifiedName());

            activationInterceptor.onSourceRegistrationActivated(m_instance);

            auto&& instance = RegistrationExtensions::getAlignedPointer(*this, m_instance, typeAliasKey);

            activationInterceptor.onRegistrationActivated(instance, typeAliasKey);

            if (instance == nullptr)
            {
                HYPODERMIC_LOG_WARN("Provided instance of type " << m_instanceType.fullyQualifiedName() << " is null");
            }

            return instance;
        }

    private:
        std::shared_ptr< T > m_instance;
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
    };

} // namespace Hypodermic