#pragma once


namespace Hypodermic
{

    template <class... TPairs>
    struct MetaMap
    {
        template <class TPair>
        struct Insert
        {
            typedef MetaMap< TPairs..., TPair > Type;
        };

        template <class TPair>
        struct Comparer
        {
            typedef typename TPair::Key Type;
        };

        static const int count = static_cast< int >(sizeof...(TPairs));
    };
    
} // namespace Hypodermic