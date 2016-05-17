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
    class IRegistration;


namespace Traits
{

    namespace Details
    {

        template <class TParent>
        struct ArgumentResolverInvoker
        {
            explicit ArgumentResolverInvoker(const IRegistration& registration, Container& container)
                : m_registration(registration)
                , m_container(container)
            {
            }

            template <class T, class = typename std::enable_if< !std::is_convertible< TParent, T >::value && IsSupportedArgument< typename std::decay< T >::type >::value >::type>
            operator T()
            {
                return ArgumentResolver< typename std::decay< T >::type >::template resolveFor< TParent >(m_registration, m_container);
            }

        private:
            const IRegistration& m_registration;
            Container& m_container;
        };


        template <class T, class TArgumentPack>
        struct ConstructorDescriptor;


        template <class T>
        struct ConstructorDescriptor< T, Utils::ArgumentPack<> >
        {
            static std::function< std::shared_ptr< T >(const IRegistration&, Container&) > describe()
            {
                return [](const IRegistration&, Container&)
                {
                    return std::make_shared< T >();
                };
            }
        };


        template <class T, class... TAnyArgument>
        struct ConstructorDescriptor< T, Utils::ArgumentPack< TAnyArgument... > >
        {
            static std::function< std::shared_ptr< T >(const IRegistration&, Container&) > describe()
            {
                return [](const IRegistration& registration, Container& container)
                {
                    return std::make_shared< T >(ArgumentResolverInvoker< typename TAnyArgument::Type >(registration, container)...);
                };
            }
        };

    }


    template <class T>
    using ConstructorDescriptor = Details::ConstructorDescriptor< T, Traits::ConstructorTypologyDeducer< T > >;

} // namespace Traits
} // namespace Hypodermic