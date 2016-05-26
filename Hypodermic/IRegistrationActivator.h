#pragma once

#include <memory>


namespace Hypodermic
{

    class ComponentContext;


    class IRegistrationActivator
    {
    public:
        virtual ~IRegistrationActivator() {}

        virtual std::shared_ptr< void > activate(ComponentContext& container) = 0;

        virtual void raiseActivated(ComponentContext& container, const std::shared_ptr< void >& instance) = 0;
    };

} // namespace Hypodermic