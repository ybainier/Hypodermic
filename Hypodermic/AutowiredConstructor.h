#pragma once

#include <type_traits>


namespace Hypodermic
{

    template <class TSignature>
    struct AutowiredConstructor
    {
        typedef TSignature Type;
    };


    namespace Details
    {
    
        template <class T>
        struct HasAutowiredSignatureMember
        {
        private:
            typedef char yes;
            typedef struct { char dummy[2]; } no;

            template <class U>
            static yes check(typename U::AutowiredSignature*);

            template <class U>
            static no check(...);

        public:
            static const bool value = sizeof(check< T >(nullptr)) == sizeof(yes);
        };

    } // namespace Details


    template <class T>
    struct HasAutowiredSignature : std::integral_constant< bool, Details::HasAutowiredSignatureMember< T >::value >
    {
    };

} // namespace Hypodermic