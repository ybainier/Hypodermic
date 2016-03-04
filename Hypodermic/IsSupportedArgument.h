#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>


namespace Hypodermic
{
namespace Traits
{

    namespace Details
    {

        template <class T>
        struct IsSupportedArgument : std::false_type {};

        template <class T>
        struct IsSupportedArgument< std::shared_ptr< T > > : std::true_type {};

        template <class T>
        struct IsSupportedArgument< std::vector< std::shared_ptr< T > > > : std::true_type {};

        template <class T>
        struct IsSupportedArgument< std::function< std::shared_ptr< T >() > > : std::true_type {};

    }


    template <class T>
    struct IsSupportedArgument : std::integral_constant
    <
        bool,
        std::is_class< T >::value && !std::is_abstract< T >::value && Details::IsSupportedArgument< T >::value
    >
    {};

} // namespace Traits
} // namespace Hypodermic
