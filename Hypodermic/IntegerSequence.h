#pragma once


namespace Hypodermic
{
namespace Utils
{

    template <int... N>
    struct IntegerSequence
    {
        using Type = IntegerSequence;

        template <int M>
        using Add = IntegerSequence< M, N... >;
    };


    namespace Details
    {

        template <int N, class TIntegerSequence>
        struct MakeIntegerSequence
        {
            static_assert(N > 0, "N should be greater than 0");

            typedef typename MakeIntegerSequence< N - 1, typename TIntegerSequence::template Add< N > >::Type Type;
        };

        template <class TIntegerSequence>
        struct MakeIntegerSequence< 0, TIntegerSequence >
        {
            typedef TIntegerSequence Type;
        };

    }

    template <int N>
    using MakeIntegerSequence = typename Details::MakeIntegerSequence< N, IntegerSequence<> >::Type;

} // namespace Utils
} // namespace Hypodermic
