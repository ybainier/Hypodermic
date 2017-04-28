#pragma once

#include <memory>


namespace Hypodermic
{
    
    class IRegistration;
    class RegistrationContext;
    class TypeAliasKey;


    class IRegistrationScope
    {
    public:
        virtual ~IRegistrationScope() {}

        virtual void addRegistration(const std::shared_ptr< IRegistration >& registration) = 0;

        virtual bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const = 0;

        virtual std::shared_ptr< IRegistrationScope > clone() = 0;
    };

} // namespace Hypodermic