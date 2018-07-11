#pragma once

#include <memory>


namespace Hypodermic
{
    
    class IRegistration;


    class IRegistrationRegistry
    {
    public:
        virtual ~IRegistrationRegistry() = default;

        virtual void addRegistration(const std::shared_ptr< IRegistration >& registration) = 0;
    };

} // namespace Hypodermic