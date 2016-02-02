#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "Hypodermic/ArgResolver.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container;


namespace Traits
{
    
    namespace Details
    {
        template <class... TArgs>
        struct ArgResolverToStringInvoker;

        template <class TArg>
        struct ArgResolverToStringInvoker< TArg >
        {
            static std::string invokeToString()
            {
                return ArgResolver< TArg >::toString();
            }
        };

        template <class TArg, class... TArgs>
        struct ArgResolverToStringInvoker< TArg, TArgs... >
        {
            static std::string invokeToString()
            {
                std::stringstream stream;
                stream << ArgResolver< TArg >::toString() << ", " << ArgResolverToStringInvoker< TArgs... >::invokeToString();

                return stream.str();
            }
        };
    }


    template <class TSignature>
    struct ConstructorDescriptor;


    template <class T>
    struct ConstructorDescriptor< T() >
    {
        static std::function< std::shared_ptr< T >(Container&) > describe()
        {
            return [](Container&)
            {
                return std::make_shared< T >();
            };
        }

        static std::string toString()
        {
            std::stringstream stream;
            stream << Utils::getMetaTypeInfo< T >().fullyQualifiedName() << "()";

            return stream.str();
        }
    };


    template <class T, class... TArgs>
    struct ConstructorDescriptor< T(TArgs...) >
    {
        static_assert(std::is_constructible< T, typename ArgResolver< TArgs >::Type... >::value, "Invalid signature for T");

        static std::function< std::shared_ptr< T >(Container&) > describe()
        {
            return [](Container& container)
            {
                return std::make_shared< T >
                (
                    ArgResolver< TArgs >::resolveFor< T >(container)...
                );
            };
        }

        static std::string toString()
        {
            std::stringstream stream;
            stream << Utils::getMetaTypeInfo< T >().fullyQualifiedName() << "(" << Details::ArgResolverToStringInvoker< TArgs... >::invokeToString() << ")";

            return stream.str();
        }
    };

} // namespace Traits
} // namespace Hypodermic