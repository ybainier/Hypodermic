#pragma once

#include <type_traits>

#include "Hypodermic/AutowiredConstructor.h"
#include "Hypodermic/DefaultConstructibleRegistrationDescriptor.h"
#include "Hypodermic/MissingConstructorRegistrationDescriptor.h"
#include "Hypodermic/ProvidedConstructorRegistrationDescriptor.h"
#include "Hypodermic/ProvidedInstanceRegistrationDescriptor.h"


namespace Hypodermic
{

    namespace RegistrationDescriptorBuilder
    {

        namespace Details
        {

            template <class T, bool HasAutowiredSignature>
            struct ForTypeConstruction;

            template <class T>
            struct ForTypeConstruction< T, false >
            {
                typedef typename std::conditional
                <
                    std::is_default_constructible< T >::value,
                    DefaultConstructibleRegistrationDescriptor< RegistrationDescriptorInfo< T > >,
                    MissingConstructorRegistrationDescriptor< RegistrationDescriptorInfo< T > >
                >
                ::type Type;
            };

            template <class T>
            struct ForTypeConstruction< T, true >
            {
                typedef ProvidedConstructorRegistrationDescriptor
                <
                    RegistrationDescriptorInfo< T >,
                    typename T::AutowiredSignature::Type
                >
                Type;
            };

        }

        template <class T>
        struct ForTypeConstruction : Details::ForTypeConstruction< T, HasAutowiredSignature< T >::value >
        {
        };

        template <class T>
        struct ForProvidedInstance
        {
            typedef ProvidedInstanceRegistrationDescriptor
            <
                RegistrationDescriptorInfo< T, InstanceLifetimes::Persistent >
            >
            Type;
        };

    } // namespace RegistrationDescriptorBuilder

} // namespace Hypodermic