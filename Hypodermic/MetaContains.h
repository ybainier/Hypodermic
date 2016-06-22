#pragma once

#include <type_traits>


namespace Hypodermic
{

    namespace Details
    {
    
        template <int N, class TSequence, class T>
        struct MetaContains;
    
        template <class TSequence, class T>
        struct MetaContains< 0, TSequence, T > : std::false_type
        {};

        template <template <class...> class TSequence, class TValue, class T>
        struct MetaContains< 1, TSequence< TValue >, T > : std::is_same< typename TSequence< TValue >::template Comparer< TValue >::Type, T >
        {};

        template <int N, template <class...> class TSequence, class TValue, class... TValues, class T>
        struct MetaContains< N, TSequence< TValue, TValues... >, T > : std::conditional
                                                                        <
                                                                            std::is_same< typename TSequence< TValue, TValues... >::template Comparer< TValue >::Type, T >::value,
                                                                            std::true_type,
                                                                            MetaContains< N - 1, TSequence< TValues... >, T >
                                                                        >::type
        {};

    } // namespace Details


    template <class TSequence, class T>
    struct MetaContains : Details::MetaContains< TSequence::count, TSequence, T >
    {};
    
} // namespace Hypodermic