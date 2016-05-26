#pragma once

#include <type_traits>

#include "Hypodermic/ConstructorTypologyDeducer.h"


namespace Hypodermic
{
namespace Traits
{

    namespace Impl
    {
        template <class T, bool IsAbstract, class = void>
        struct HasAutowireableConstructor;

        template <class T>
        struct HasAutowireableConstructor< T, true, void > : std::false_type {};

        template <class T>
        struct HasAutowireableConstructor
        <
            T,
            false,
            typename std::enable_if< !std::is_same< ConstructorTypologyDeducer< T >, ConstructorTypologyNotSupported >::value >::type
        > : std::true_type
        {};


        template <class T>
        struct HasAutowireableConstructor
        <
            T,
            false,
            typename std::enable_if< std::is_same< ConstructorTypologyDeducer< T >, ConstructorTypologyNotSupported >::value >::type
        > : std::false_type
        {};

    }


    template <class T>
    using HasAutowireableConstructor = Impl::HasAutowireableConstructor< T, std::is_class< T >::value && std::is_abstract< T >::value >;

} // namespace Traits
} // namespace Hypodermic
