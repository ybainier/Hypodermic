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
        virtual ~IRegistrationScope() = default;

        virtual void addRegistration(const std::shared_ptr< IRegistration >& registration) = 0;

        virtual bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const = 0;

        virtual void copyTo(IRegistrationScope& other) const = 0;
        virtual void addRegistrationContext(const std::shared_ptr< RegistrationContext >& registrationContext) = 0;
    };

} // namespace Hypodermic