#pragma once


namespace Hypodermic
{

    template <class TSequence, class TValue>
    struct MetaInsert
    {
        typedef typename TSequence::template Insert< TValue >::Type Type;
    };
    
} // namespace Hypodermic