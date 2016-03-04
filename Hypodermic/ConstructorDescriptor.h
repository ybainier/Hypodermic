#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "Hypodermic/ArgumentPack.h"
#include "Hypodermic/ArgumentResolver.h"
#include "Hypodermic/ConstructorTypologyDeducer.h"
#include "Hypodermic/IsSupportedArgument.h"


namespace Hypodermic
{

    class Container;


namespace Traits
{

    namespace Details
    {

        template <class TParent>
        struct ArgumentResolverInvoker
        {
            explicit ArgumentResolverInvoker(Container& container)
                : m_container(container)
            {
            }

            template <class T, class = typename std::enable_if< !std::is_convertible< TParent, T >::value && IsSupportedArgument< typename std::decay< T >::type >::value >::type>
            operator T()
            {
                return ArgumentResolver< typename std::decay< T >::type >::template resolveFor< TParent >(m_container);
            }

        private:
            Container& m_container;
        };


        template <class T, class TArgumentPack>
        struct ConstructorDescriptor;


        template <class T>
        struct ConstructorDescriptor< T, Utils::ArgumentPack<> >
        {
            static std::function< std::shared_ptr< T >(Container&) > describe()
            {
                return [](Container&)
                {
                    return std::make_shared< T >();
                };
            }
        };


        template <class T, class... TAnyArgument>
        struct ConstructorDescriptor< T, Utils::ArgumentPack< TAnyArgument... > >
        {
            static std::function< std::shared_ptr< T >(Container&) > describe()
            {
                return [](Container& container)
                {
                    return std::make_shared< T >(ArgumentResolverInvoker< typename TAnyArgument::Type >(container)...);
                };
            }
        };

    }


    template <class T>
    using ConstructorDescriptor = Details::ConstructorDescriptor< T, Traits::ConstructorTypologyDeducer< T > >;

} // namespace Traits
} // namespace Hypodermic