#pragma once

#include <memory>

#include "Hypodermic/IRegistrationRegistry.h"


namespace Hypodermic
{
    
    class IMutableRegistrationScope;
    class IRegistration;
    class RegistrationContext;
    class TypeAliasKey;


    class IRegistrationScope : public IRegistrationRegistry
    {
    public:
        virtual ~IRegistrationScope() = default;

        virtual bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const = 0;

        virtual void copyTo(IMutableRegistrationScope& other) const = 0;
    };

} // namespace Hypodermic