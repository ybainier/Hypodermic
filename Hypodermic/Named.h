#pragma once

#include <type_traits>

#include "Hypodermic/EnforceBaseOf.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{
namespace RegistrationDescriptorOperations
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class Named
    {
    private:
        typedef typename TDescriptorInfo::InstanceType InstanceType;

    public:
        // This template avoids Early Template Instantiation issue
        template <class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::SelfRegistered::Type
        >
        ::Type& named(const std::string& name)
        {
            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->addTypeIfMissing(createKeyForNamedType< InstanceType >(name));

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::SelfRegistered::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

        // This template avoids Early Template Instantiation issue
        template <class TBase, class TDelayedDescriptor = TDescriptor>
        typename std::enable_if
        <
            !std::is_same< TBase, InstanceType >::value,
            typename TDelayedDescriptor::template UpdateDescriptor
            <
                typename TDescriptorInfo::template RegisterBase< TBase >::Type
            >
            ::Type&
        >
        ::type named(const std::string& name)
        {
            Extensions::EnforceBaseOf< TDescriptorInfo, TBase, InstanceType >::act();

            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->addTypeIfMissing(createKeyForNamedType< TBase >(name), [](const std::shared_ptr< void >& x)
            {
                auto instanceDynamicType = std::static_pointer_cast< InstanceType >(x);
                auto instanceStaticType = std::static_pointer_cast< TBase >(instanceDynamicType);
                return instanceStaticType;
            });

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::template RegisterBase< TBase >::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~Named() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic