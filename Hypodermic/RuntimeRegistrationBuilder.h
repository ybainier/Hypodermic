#pragma once

#include "Hypodermic/ActivationHandlers.h"
#include "Hypodermic/DependencyFactories.h"
#include "Hypodermic/InstanceFactory.h"
#include "Hypodermic/IRuntimeRegistrationBuilder.h"
#include "Hypodermic/Registration.h"
#include "Hypodermic/TypeAliases.h"


namespace Hypodermic
{

    class RuntimeRegistrationBuilder : public IRuntimeRegistrationBuilder
    {
    public:
        std::shared_ptr< IRegistration > build(const TypeInfo& instanceType, const InstanceFactory& instanceFactory) override
        {
            return std::make_shared< Registration >
            (
                instanceType,
                TypeAliases(),
                instanceFactory,
                DependencyFactories(),
                ActivationHandlers(),
                false /* not a fallback registration */
            );
        }
    };

} // namespace Hypodermic