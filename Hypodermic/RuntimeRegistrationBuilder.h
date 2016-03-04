#pragma once

#include <unordered_map>

#include "Hypodermic/IRuntimeRegistrationBuilder.h"
#include "Hypodermic/Registration.h"


namespace Hypodermic
{

    class RuntimeRegistrationBuilder : public IRuntimeRegistrationBuilder
    {
    public:
        std::shared_ptr< IRegistration > build(const TypeInfo& instanceType, const std::function< std::shared_ptr< void >(Container&) >& instanceFactory) override
        {
            return std::make_shared< Registration >
            (
                instanceType,
                std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >(),
                instanceFactory,
                std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >(),
                std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >()
            );
        }
    };

} // namespace Hypodermic