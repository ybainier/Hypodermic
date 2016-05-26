#pragma once

#include <type_traits>

#include "Hypodermic/AutowireableConstructor.h"
#include "Hypodermic/AutowireableConstructorRegistrationDescriptor.h"
#include "Hypodermic/ProvidedInstanceFactoryRegistrationDescriptor.h"
#include "Hypodermic/ProvidedInstanceRegistrationDescriptor.h"


namespace Hypodermic
{

    namespace RegistrationDescriptorBuilder
    {

        template <class T>
        struct ForTypeConstruction
        {
            static_assert(Traits::HasAutowireableConstructor< T >::value, "Could not autowire T: you should consider registering T either by providing an instance or an instance factory");

            typedef AutowireableConstructorRegistrationDescriptor< RegistrationDescriptorInfo< T > > Type;
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
                static yes test(decltype(std::declval< T >()(std::declval< ComponentContext& >()))*);

                template <class T>
                static no test(...);

                static bool const value = sizeof(test< TFunctor >(nullptr)) == sizeof(yes);
            };

            template <class TFunctor>
            struct GetResultType
            {
                typedef decltype(std::declval< TFunctor >()(std::declval< ComponentContext& >())) Type;
            };

            template <class TFunctor>
            struct IsResultASharedPtr : IsSharedPtr< typename GetResultType< TFunctor >::Type >
            {
            };

        public:
            static_assert(IsInvokableWithContainer< TCallable >::value, "TCallable should have an invokable function which signature matches std::shared_ptr< T >(ComponentContext&)");
            static_assert(IsResultASharedPtr< TCallable >::value, "TCallable should return a std:shared_ptr");

            typedef typename GetResultType< TCallable >::Type ResultType;
            typedef typename ResultType::element_type InstanceType;

            typedef ProvidedInstanceFactoryRegistrationDescriptor< RegistrationDescriptorInfo< InstanceType > > Type;
        };

    } // namespace RegistrationDescriptorBuilder

} // namespace Hypodermic