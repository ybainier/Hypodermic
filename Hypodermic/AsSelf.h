#pragma once

#include <type_traits>

#include "Hypodermic/InstanceRegistrationTags.h"


namespace Hypodermic
{
namespace RegistrationDescriptorOperations
{
    
    namespace Details
    {

        struct InstanceRegistrationNotResolvable {};
        struct InstanceRegistrationResolvable {};
        
        template <class TDescriptorInfo>
        struct GetInstanceRegistrationResolvability
        {
        private:
            typedef typename TDescriptorInfo::RegisteredBases RegisteredBases;
            typedef typename TDescriptorInfo::SelfRegistrationTag InstanceRegistrationTag;

        public:
            typedef typename std::conditional
            <
                (RegisteredBases::count == 0) || std::is_same< InstanceRegistrationTag, Tags::SelfRegistered >::value,
                InstanceRegistrationResolvable,
                InstanceRegistrationNotResolvable
            >
            ::type Type;
        };

    } // namespace Details


    template
    <
        class TDescriptor,
        class TDescriptorInfo,
        class TInstanceRegistrationResolvability = typename Details::GetInstanceRegistrationResolvability< TDescriptorInfo >::Type
    >
    class AsSelf;


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class AsSelf< TDescriptor, TDescriptorInfo, Details::InstanceRegistrationNotResolvable >
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
        virtual ~AsSelf() = default;
    };


    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class AsSelf< TDescriptor, TDescriptorInfo, Details::InstanceRegistrationResolvable >
    {
    protected:
        virtual ~AsSelf() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic