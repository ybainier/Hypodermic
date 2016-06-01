#pragma once

#include "Hypodermic/Container.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class ContainerInstanceRegistrationActivator : public IRegistrationActivator
    {
    public:
        ContainerInstanceRegistrationActivator(const IRegistration& registration, const std::weak_ptr< Container >& instance)
            : m_registration(registration)
            , m_instance(instance)
        {
        }
    
        std::shared_ptr< void > activate(IResolutionContext&) override
        {
            HYPODERMIC_LOG_INFO("Activating Container instance of type " << m_registration.instanceType().fullyQualifiedName());

            return m_instance.lock();
        }

        void raiseActivated(ComponentContext&, const std::shared_ptr< void >&) override
        {
        }

    private:
        const IRegistration& m_registration;
        std::weak_ptr< Container > m_instance;
    };

} // namespace Hypodermic