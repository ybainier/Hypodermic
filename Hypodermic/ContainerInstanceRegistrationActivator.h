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

    class ContainerInstanceRegistrationActivator : public IRegistrationActivator,
                                                   public NoopRegistrationActivationInterceptor
    {
    public:
        ContainerInstanceRegistrationActivator(const IRegistration& registration, const std::weak_ptr< Container >& instance)
            : m_registration(registration)
            , m_instance(instance)
        {
        }
    
        std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            return activate(*this, container, typeAliasKey);
        }

        std::shared_ptr< void > activate(IRegistrationActivationInterceptor&, Container&, const TypeAliasKey&) override
        {
            HYPODERMIC_LOG_INFO("Activating Container instance of type " << m_registration.instanceType().fullyQualifiedName());

            std::shared_ptr< void > instance = m_instance.lock();
            if (instance == nullptr)
                HYPODERMIC_LOG_WARN("Container instance of type " << m_registration.instanceType().fullyQualifiedName() << " is null");

            return instance;
        }

    private:
        const IRegistration& m_registration;
        std::weak_ptr< Container > m_instance;
    };

} // namespace Hypodermic