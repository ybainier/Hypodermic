#pragma once

#include <memory>


namespace Hypodermic
{

    class IRegistration;
    class IResolutionScope;

    
    class RegistrationContext
    {
    public:
        RegistrationContext(IResolutionScope& scope, const std::shared_ptr< IRegistration >& registration)
            : m_scope(scope)
            , m_registration(registration)
        {
        }

        IResolutionScope& scope() const
        {
            return m_scope;
        }

        const std::shared_ptr< IRegistration >& registration() const
        {
            return m_registration;
        }

    private:
        IResolutionScope& m_scope;
        std::shared_ptr< IRegistration > m_registration;
    };

} // namespace Hypodermic