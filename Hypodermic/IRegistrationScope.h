#pragma once

#include <memory>


namespace Hypodermic
{
    
    class IRegistration;
    class TypeAliasKey;


    class IRegistrationScope
    {
    public:
        virtual ~IRegistrationScope() {}

        virtual void addRegistration(const std::shared_ptr< IRegistration >& registration) = 0;

        virtual bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< IRegistration > >& registrations) const = 0;

        virtual bool isEmpty() const = 0;
    };

} // namespace Hypodermic