#pragma once

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
    class As
    {
    private:
        typedef typename TDescriptorInfo::InstanceType InstanceType;

    public:
        template <class TBase, class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterBase< TBase >::Type
        >
        ::Type& as()
        {
            Extensions::EnforceBaseOf< TDescriptorInfo, TBase, InstanceType >::act();

            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->addTypeIfMissing(createKeyForType< TBase >(), [](const std::shared_ptr< void >& x)
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
        virtual ~As() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic