#pragma once

#include <memory>

#include "Hypodermic/InstanceFactory.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class IRuntimeRegistrationBuilder
    {
    public:
        virtual ~IRuntimeRegistrationBuilder() = default;

        virtual std::shared_ptr< IRegistration > build(const TypeInfo& instanceType, const InstanceFactory& instanceFactory) = 0;
    };

} // namespace Hypodermic