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
    class ProvidedInstanceRegistrationActivator : public IRegistrationActivator,
                                                  public NoopRegistrationActivationInterceptor
    {
    public:
        ProvidedInstanceRegistrationActivator(const IRegistration& registration, const std::shared_ptr< T >& instance)
            : m_registration(registration)
            , m_instance(instance)
        {
        }
    
        std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            return activate(*this, container, typeAliasKey);
        }

        std::shared_ptr< void > activate(IRegistrationActivationInterceptor& activationInterceptor, Container&, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating provided instance of type " << m_registration.instanceType().fullyQualifiedName());

            activationInterceptor.onSourceRegistrationActivated(m_instance);

            auto&& instance = RegistrationExtensions::getAlignedPointer(m_registration, m_instance, typeAliasKey);

            activationInterceptor.onRegistrationActivated(instance, typeAliasKey);

            if (instance == nullptr)
            {
                HYPODERMIC_LOG_WARN("Provided instance of type " << m_registration.instanceType().fullyQualifiedName() << " is null");
            }

            return instance;
        }

    private:
        const IRegistration& m_registration;
        std::shared_ptr< T > m_instance;
    };

} // namespace Hypodermic