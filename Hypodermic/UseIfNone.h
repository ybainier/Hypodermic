#pragma once

#include <type_traits>

#include "Hypodermic/InstanceRegistrationTags.h"


namespace Hypodermic
{
namespace RegistrationDescriptorOperations
{
    
    template
    <
        class TDescriptor,
        class TDescriptorInfo,
        class TRegistrationBehavior = typename TDescriptorInfo::Behavior
    >
    class UseIfNone;


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class UseIfNone< TDescriptor, TDescriptorInfo, Tags::DefaultRegistration >
    {
    public:
        // This template avoids Early Template Instantiation issue
        template <class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::PreserveDefault::Type
        >
        ::Type& useIfNone()
        {
            auto descriptor = static_cast< TDescriptor* >(this);

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::PreserveDefault::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~UseIfNone() {}
    };


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class UseIfNone< TDescriptor, TDescriptorInfo, Tags::FallbackRegistration >
    {
    protected:
        virtual ~UseIfNone() {}
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic