#pragma once

#include <functional>
#include <memory>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container;


    class IRuntimeRegistrationBuilder
    {
    public:
        virtual ~IRuntimeRegistrationBuilder() {}

        virtual std::shared_ptr< IRegistration > build(const TypeInfo& instanceType, const std::function< std::shared_ptr< void >(Container&) >& instanceFactory) = 0;
    };

} // namespace Hypodermic