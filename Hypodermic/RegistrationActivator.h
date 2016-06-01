#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <boost/signals2.hpp>

#include "Hypodermic/ActivationHandlers.h"
#include "Hypodermic/InstanceFactory.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationActivator.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class RegistrationActivator : public IRegistrationActivator
    {
    public:
        RegistrationActivator(const IRegistration& registration,
                              const InstanceFactory& instanceFactory,
                              const ActivationHandlers& activationHandlers)
            : m_registration(registration)
            , m_instanceFactory(instanceFactory)
        {
            for (auto&& handler : activationHandlers)
                m_activated.connect(handler);
        }

        std::shared_ptr< void > activate(IResolutionContext& resolutionContext) override
        {
            HYPODERMIC_LOG_INFO("Activating type " << m_registration.instanceType().fullyQualifiedName());

            if (!m_instanceFactory)
            {
                HYPODERMIC_LOG_WARN("No instance factory provided to activate type " << m_registration.instanceType().fullyQualifiedName());
                return nullptr;
            }

            return m_instanceFactory(m_registration, resolutionContext);
        }

        void raiseActivated(ComponentContext& container, const std::shared_ptr< void >& instance) override
        {
            m_activated(container, instance);
        }

    private:
        const IRegistration& m_registration;
        InstanceFactory m_instanceFactory;
        boost::signals2::signal< void(ComponentContext&, const std::shared_ptr< void >&) > m_activated;
    };

} // namespace Hypodermic