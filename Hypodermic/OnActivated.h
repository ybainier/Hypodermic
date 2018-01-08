#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class ComponentContext;

namespace RegistrationDescriptorOperations
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class OnActivated
    {
    private:
        typedef typename TDescriptorInfo::InstanceType InstanceType;

    public:
        // This template avoids Early Template Instantiation issue
        template <class TDelayedDescriptor = TDescriptor>
        TDelayedDescriptor& onActivated(const std::function< void(ComponentContext&, const std::shared_ptr< InstanceType >&) >& handler)
        {
            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->addActivationHandler([handler](ComponentContext& componentContext, const std::shared_ptr< void >& instance)
            {
                return handler(componentContext, std::static_pointer_cast< InstanceType >(instance));
            });

            return *descriptor;
        }

    protected:
        virtual ~OnActivated() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic