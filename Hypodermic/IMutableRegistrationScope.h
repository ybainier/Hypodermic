#pragma once

#include <memory>

#include "Hypodermic/IRegistrationScope.h"


namespace Hypodermic
{

    class RegistrationContext;
    class TypeAliasKey;


    class IMutableRegistrationScope : public IRegistrationScope
    {
    public:
        virtual ~IMutableRegistrationScope() = default;

        virtual void addRegistrationContext(const TypeAliasKey& typeAliasKey, const std::shared_ptr< RegistrationContext >& registrationContext) = 0;
    };

} // namespace Hypodermic