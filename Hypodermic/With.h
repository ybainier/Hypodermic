#pragma once

#include <type_traits>


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

        template <class TBase, class T>
        struct EnforceBaseOf
        {
            static_assert(std::is_base_of< TBase, T >::value && !std::is_same< TBase, T >::value, "TBase should be a base of T");

            static void act() {}
        };

        template <class TDependency>
        struct EnforceDependencyNotAlreadyRegistered
        {
            template <class T>
            struct Act
            {
                static_assert(!TDescriptorInfo::template IsDependencyRegistered< TDependency >::value, "TDependency is already registered for instance T");

                typedef void Type;
            };

            static typename Act< typename TDescriptorInfo::InstanceType >::Type act() {}
        };

    public:
        template <class TDependency, class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterDependencyFactory< TDependency >::Type
        >
        ::Type& with(const std::function< std::shared_ptr< TDependency >(ComponentContext&) >& factory)
        {
            EnforceDependencyNotAlreadyRegistered< TDependency >::act();

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
            EnforceBaseOf< TDependency, TProvidedDependency >::act();

            EnforceDependencyNotAlreadyRegistered< TDependency >::act();

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
            EnforceBaseOf< TDependency, TProvidedDependency >::act();
            EnforceDependencyNotAlreadyRegistered< TDependency >::act();

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