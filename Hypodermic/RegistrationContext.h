#pragma once

#include <memory>


namespace Hypodermic
{

    class IRegistration;
    class IResolutionContainer;

    
    class RegistrationContext
    {
    public:
        RegistrationContext(IResolutionContainer& resolutionContainer, const std::shared_ptr< IRegistration >& registration)
            : m_resolutionContainer(resolutionContainer)
            , m_registration(registration)
        {
        }

        IResolutionContainer& resolutionContainer() const
        {
            return m_resolutionContainer;
        }

        const std::shared_ptr< IRegistration >& registration() const
        {
            return m_registration;
        }

    private:
        IResolutionContainer& m_resolutionContainer;
        std::shared_ptr< IRegistration > m_registration;
    };

} // namespace Hypodermic