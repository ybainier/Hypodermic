#pragma once

#include <type_traits>
#include <utility>

#include "Hypodermic/Config.h"


namespace Hypodermic
{
namespace Traits
{

#if defined(HYPODERMIC_MODERN_COMPILER)

    // From https://blogs.msdn.microsoft.com/vcblog/2015/12/02/partial-support-for-expression-sfinae-in-vs-2015-update-1/
    // Thanks to Bat-Ulzii Luvsanbat

    template <class T, class = void>
    struct IsComplete : std::false_type
    {};

    template <class T>
    struct IsComplete< T, decltype(void(sizeof(T))) > : std::true_type
    {};

#else // VS2013

    namespace Details
    {

        // Inspired from the internets and http://stackoverflow.com/questions/25796126/static-assert-that-template-typename-t-is-not-complete
        // So far, no one got it working for both complete and abstract types

        template <class T>
        struct IsComplete
        {
            typedef char no;
            struct yes { char dummy[2]; };

            template <class U, class = decltype(sizeof(std::declval< U >())) >
            static yes check(U*);

            template <class U>
            static no check(...);

            static const bool value = sizeof(check< T >(nullptr)) == sizeof(yes);
        };

    } // namespace Details

    
    template <class T>
    struct IsComplete : std::integral_constant< bool, Details::IsComplete< T >::value >
    {};

#endif
 
} // Traits   
} // namespace Hypodermic