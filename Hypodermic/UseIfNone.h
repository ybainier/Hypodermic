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
        class TFallbackRegistrationTag = typename TDescriptorInfo::FallbackRegistrationTag
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
            typename TDescriptorInfo::UseIfNone::Type
        >
        ::Type& useIfNone()
        {
            auto descriptor = static_cast< TDescriptor* >(this);

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::UseIfNone::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~UseIfNone() = default;
    };


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class UseIfNone< TDescriptor, TDescriptorInfo, Tags::FallbackRegistration >
    {
    protected:
        virtual ~UseIfNone() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic