#pragma once

#include "Hypodermic/InstanceRegistrationTags.h"


namespace Hypodermic
{
namespace RegistrationDescriptorOperations
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo,
        class TInstanceRegistrationTag = typename TDescriptorInfo::InstanceRegistrationTag
    >
    class AsSelf;


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class AsSelf< TDescriptor, TDescriptorInfo, Tags::NotSelfRegistered >
    {
    public:
        // This template avoids Early Template Instantiation issue
        template <class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::SelfRegistered::Type
        >
        ::Type& asSelf()
        {
            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->addTypeIfMissing(createKeyForType< typename TDescriptorInfo::InstanceType >());

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::SelfRegistered::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~AsSelf() {}
    };


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class AsSelf< TDescriptor, TDescriptorInfo, Tags::SelfRegistered >
    {
    protected:
        virtual ~AsSelf() {}
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic