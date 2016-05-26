#pragma once

#include <memory>

#include "Hypodermic/IRegistration.h"


namespace Hypodermic
{
    
    class ActivatedRegistrationInfo
    {
    public:
        ActivatedRegistrationInfo(const std::shared_ptr< IRegistration >& registration, const std::shared_ptr< void >& instance)
            : m_registration(registration)
            , m_instance(instance)
        {
        }

        const std::shared_ptr< IRegistration >& registration() const
        {
            return m_registration;
        }

        const std::shared_ptr< void >& instance() const
        {
            return m_instance;
        }

    private:
        const std::shared_ptr< IRegistration >& m_registration;
        std::shared_ptr< void > m_instance;
    };

} // namespace Hypodermic