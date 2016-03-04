#pragma once

#include <type_traits>

#include "Hypodermic/IsSupportedArgument.h"


namespace Hypodermic
{
namespace Traits
{

    template <class TParent>
    struct AnyArgument
    {
        typedef TParent Type;

        template <class T, class = typename std::enable_if< !std::is_convertible< TParent, T >::value && IsSupportedArgument< typename std::decay< T >::type >::value >::type >
        operator T()
        {
            // Nothing to do, it is only used to evaluate T kind of statically
        }
    };

} // namespace Traits
} // namespace Hypodermic
