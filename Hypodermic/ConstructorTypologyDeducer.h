#pragma once

#include <type_traits>

#include "Hypodermic/AnyArgument.h"
#include "Hypodermic/ArgumentPack.h"
#include "Hypodermic/Config.h"
#include "Hypodermic/IntegerSequence.h"


namespace Hypodermic
{
namespace Traits
{

    struct ConstructorTypologyNotSupported
    {
        typedef ConstructorTypologyNotSupported Type;
    };


    namespace Details
    {
        template <class T, int>
        using WrapAndGet = T;

        template <class, class, class = void>
        struct ConstructorTypologyDeducer;

        template <class T>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence<>,
            typename std::enable_if< !std::is_constructible< T >::value >::type
        > : ConstructorTypologyNotSupported
        {};

        template <class T>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence<>,
            typename std::enable_if< std::is_constructible< T >::value >::type
        > : Utils::ArgumentPack<>
        {};

        template <class T>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< 1 >,
            typename std::enable_if< std::is_constructible< T, AnyArgument< T > >::value >::type
        > : Utils::ArgumentPack< AnyArgument< T > >
        {};

        template <class T>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< 1 >,
            typename std::enable_if< !std::is_constructible< T, AnyArgument< T > >::value >::type
        > : std::conditional
            <
                std::is_constructible< T, AnyArgument< T > >::value,
                Utils::ArgumentPack< AnyArgument< T > >,
                typename ConstructorTypologyDeducer< T, Utils::MakeIntegerSequence< 0 > >::Type
            >::type
        {};

        template <class T, int... NthArgument>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< NthArgument... >,
            typename std::enable_if< (sizeof...(NthArgument) > 1) && std::is_constructible< T, WrapAndGet< AnyArgument< T >, NthArgument >... >::value >::type
        > : Utils::ArgumentPack< WrapAndGet< AnyArgument< T >, NthArgument >... >
        {};

        template <class T, int... NthArgument>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< NthArgument... >,
            typename std::enable_if< (sizeof...(NthArgument) > 1) && !std::is_constructible< T, WrapAndGet< AnyArgument< T >, NthArgument >... >::value >::type
        > : std::conditional
            <
                std::is_constructible< T, WrapAndGet< AnyArgument< T >, NthArgument >... >::value,
                Utils::ArgumentPack< WrapAndGet< AnyArgument< T >, NthArgument >... >,
                typename ConstructorTypologyDeducer< T, Utils::MakeIntegerSequence< sizeof...(NthArgument) - 1 > >::Type
            >::type
        {};

    }


    template <class T>
    using ConstructorTypologyDeducer = typename Details::ConstructorTypologyDeducer< T, Utils::MakeIntegerSequence< HYPODERMIC_CONSTRUCTOR_ARGUMENT_COUNT > >::Type;

} // namespace Traits
} // namespace Hypodermic