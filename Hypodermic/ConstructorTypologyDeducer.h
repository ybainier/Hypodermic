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

        template <int... N>
        struct Cardinality
        {
            static const int value = sizeof...(N);
        };

        template <class T, int>
        struct WrapAndGet : AnyArgument< T > {};

        template <class, class, class = void>
        struct ConstructorTypologyDeducer;


        // Initial recursion state
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
            Utils::IntegerSequence<>,
            typename std::enable_if< !std::is_constructible< T >::value >::type
        > : ConstructorTypologyDeducer< T, Utils::MakeIntegerSequence< 1 > >::Type
        {};


        // Common recusion state
        template <class T, int... NthArgument>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< NthArgument... >,
            typename std::enable_if
            <
                (Cardinality< NthArgument... >::value > 0 && Cardinality< NthArgument... >::value < HYPODERMIC_CONSTRUCTOR_ARGUMENT_COUNT) &&
                std::is_constructible< T, WrapAndGet< T, NthArgument >... >::value
            >::type
        > : Utils::ArgumentPack< WrapAndGet< T, NthArgument >... >
        {};

        template <class T, int... NthArgument>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< NthArgument... >,
            typename std::enable_if
            <
                (Cardinality< NthArgument... >::value > 0 && Cardinality< NthArgument... >::value < HYPODERMIC_CONSTRUCTOR_ARGUMENT_COUNT) &&
                !std::is_constructible< T, WrapAndGet< T, NthArgument >... >::value
            >::type
        > : ConstructorTypologyDeducer< T, Utils::MakeIntegerSequence< sizeof...(NthArgument) + 1 > >::Type
        {};


        // Last recursion state
        template <class T, int... NthArgument>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< NthArgument... >,
            typename std::enable_if< (Cardinality< NthArgument... >::value == HYPODERMIC_CONSTRUCTOR_ARGUMENT_COUNT) && std::is_constructible< T, WrapAndGet< T, NthArgument >... >::value >::type
        > : Utils::ArgumentPack< WrapAndGet< T, NthArgument >... >
        {};

        template <class T, int... NthArgument>
        struct ConstructorTypologyDeducer
        <
            T,
            Utils::IntegerSequence< NthArgument... >,
            typename std::enable_if< (Cardinality< NthArgument... >::value == HYPODERMIC_CONSTRUCTOR_ARGUMENT_COUNT) && !std::is_constructible< T, WrapAndGet< T, NthArgument >... >::value >::type
        > : ConstructorTypologyNotSupported
        {};

    } // namespace Details


    template <class T>
    using ConstructorTypologyDeducer = typename Details::ConstructorTypologyDeducer< T, Utils::MakeIntegerSequence< 0 > >::Type;

} // namespace Traits
} // namespace Hypodermic