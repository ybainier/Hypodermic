#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class Container;

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
        TDelayedDescriptor& onActivated(const std::function< void(Container&, const std::shared_ptr< InstanceType >&) >& handler)
        {
            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->addActivationHandler([handler](Container& container, const std::shared_ptr< void >& instance)
            {
                return handler(container, std::static_pointer_cast< InstanceType >(instance));
            });

            return *descriptor;
        }

    protected:
        virtual ~OnActivated() {}
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic