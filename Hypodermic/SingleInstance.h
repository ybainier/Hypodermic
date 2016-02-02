#pragma once

#include "Hypodermic2/InstanceLifetime.h"


namespace Hypodermic2
{
namespace RegistrationDescriptorOperations
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo,
        class TInstanceLifetime = typename TDescriptorInfo::InstanceLifetime
    >
    class SingleInstance;


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class SingleInstance< TDescriptor, TDescriptorInfo, TransientInstance >
    {
    public:
        // This template avoids Early Template Instantiation issue
        template <class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::SingleInstance::Type
        >
        ::Type& singleInstance()
        {
            auto descriptor = static_cast< TDescriptor* >(this);

            auto updatedDescriptor = descriptor->createUpdate< typename TDescriptorInfo::SingleInstance::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~SingleInstance() {}
    };


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class SingleInstance< TDescriptor, TDescriptorInfo, PersistentInstance >
    {
    protected:
        virtual ~SingleInstance() {}
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic2