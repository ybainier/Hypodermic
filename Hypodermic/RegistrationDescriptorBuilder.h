#pragma once

#include <type_traits>

#include "Hypodermic/AutowiredConstructor.h"
#include "Hypodermic/DefaultConstructibleRegistrationDescriptor.h"
#include "Hypodermic/MissingConstructorRegistrationDescriptor.h"
#include "Hypodermic/ProvidedConstructorRegistrationDescriptor.h"
#include "Hypodermic/ProvidedInstanceFactoryRegistrationDescriptor.h"
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
            typedef ProvidedInstanceRegistrationDescriptor< RegistrationDescriptorInfo< T, InstanceLifetimes::Persistent > > Type;
        };


        template <class TCallable>
        struct ForProvidedInstanceFactory
        {
        private:
            template <class T>
            struct IsSharedPtr : std::false_type {};

            template <class T>
            struct IsSharedPtr< std::shared_ptr< T > > : std::true_type {};

            template <class TFunctor>
            struct IsInvokableWithContainer
            {
                typedef char yes;
                typedef struct { char dummy[2]; } no;

                template <class T>
                static yes test(decltype(std::declval< T >()(std::declval< Container& >()))*);

                template <class T>
                static no test(...);

                static bool const value = sizeof(test< TFunctor >(nullptr)) == sizeof(yes);
            };

            template <class TFunctor>
            struct GetResultType
            {
                typedef decltype(std::declval< TFunctor >()(std::declval< Container >())) Type;
            };

            template <class TFunctor>
            struct IsResultASharedPtr : IsSharedPtr< typename GetResultType< TFunctor >::Type >
            {
            };

        public:
            static_assert(IsInvokableWithContainer< TCallable >::value, "TCallable should have an invokable function which signature matches std::shared_ptr< T >(Container&)");
            static_assert(IsResultASharedPtr< TCallable >::value, "TCallable should return a std:shared_ptr");

            typedef typename GetResultType< TCallable >::Type ResultType;
            typedef typename ResultType::element_type InstanceType;

            typedef ProvidedInstanceFactoryRegistrationDescriptor< RegistrationDescriptorInfo< InstanceType > > Type;
        };

    } // namespace RegistrationDescriptorBuilder

} // namespace Hypodermic