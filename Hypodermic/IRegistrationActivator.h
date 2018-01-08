#pragma once

#include <memory>


namespace Hypodermic
{

    class ComponentContext;
    class IResolutionContext;


    class IRegistrationActivator
    {
    public:
        virtual ~IRegistrationActivator() = default;

        virtual std::shared_ptr< void > activate(IResolutionContext& resolutionContext) = 0;

        virtual void raiseActivated(ComponentContext& container, const std::shared_ptr< void >& instance) = 0;
    };

} // namespace Hypodermic