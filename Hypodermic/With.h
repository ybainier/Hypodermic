#pragma once

#include "Hypodermic/EnforceBaseOf.h"
#include "Hypodermic/EnforceDependencyNotAlreadyRegistered.h"


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
    class With
    {
    private:
        typedef typename TDescriptorInfo::InstanceType InstanceType;

    public:
        template <class TDependency, class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterDependencyFactory< TDependency >::Type
        >
        ::Type& with(const std::function< std::shared_ptr< TDependency >(ComponentContext&) >& factory)
        {
            Extensions::EnforceDependencyNotAlreadyRegistered< TDescriptorInfo, TDependency >::act();

            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->template addDependencyFactory< TDependency >
            (
                [factory](ComponentContext& c)
                {
                    return std::static_pointer_cast< void >(factory(c));
                }
            );

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::template RegisterDependencyFactory< TDependency >::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

        template <class TDependency, class TProvidedDependency, class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterDependencyInstance< TDependency, TProvidedDependency >::Type
        >
        ::Type& with(const std::shared_ptr< TProvidedDependency >& providedInstance)
        {
			Extensions::EnforceBaseOf< TDescriptorInfo, TDependency, TProvidedDependency >::act();
			Extensions::EnforceDependencyNotAlreadyRegistered< TDescriptorInfo, TDependency >::act();

            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->template addDependencyFactory< TDependency >
            (
                [providedInstance](ComponentContext&)
                {
                    return std::static_pointer_cast< void >(providedInstance);
                }
            );

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::template RegisterDependencyInstance< TDependency, TProvidedDependency >::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

        template <class TDependency, class TProvidedDependency, class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterDependency< TDependency, TProvidedDependency >::Type
        >
        ::Type& with()
        {
            Extensions::EnforceBaseOf< TDescriptorInfo, TDependency, TProvidedDependency >::act();
            Extensions::EnforceDependencyNotAlreadyRegistered< TDescriptorInfo, TDependency >::act();

            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->template addDependencyFactory< TDependency >([](ComponentContext& c)
            {
                return std::static_pointer_cast< void >(c.resolve< TProvidedDependency >());
            });

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::template RegisterDependency< TDependency, TProvidedDependency >::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~With() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic