#pragma once

#include "Hypodermic/InstanceLifetime.h"


namespace Hypodermic
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

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::SingleInstance::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~SingleInstance() = default;
    };


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class SingleInstance< TDescriptor, TDescriptorInfo, PersistentInstance >
    {
    protected:
        virtual ~SingleInstance() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic