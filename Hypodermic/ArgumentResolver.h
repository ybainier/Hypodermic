#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container;


namespace Traits
{

    template <class TArg>
    struct ArgumentResolver;


    template <class TArg>
    struct ArgumentResolver< std::shared_ptr< TArg > >
    {
        typedef std::shared_ptr< TArg > Type;

        template <class T>
        static Type resolveFor(Container& container)
        {
            auto&& factory = container.getDependencyFactory< T, TArg >();
            if (!factory)
                return container.resolve< TArg >();

            return factory(container);
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::vector< std::shared_ptr< TArg > > >
    {
        typedef std::vector< std::shared_ptr< TArg > > Type;

        template <class>
        static Type resolveFor(Container& container)
        {
            return container.resolveAll< TArg >();
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::function< std::shared_ptr< TArg >() > >
    {
        typedef std::function< std::shared_ptr< TArg >() > Type;

        template <class T>
        static Type resolveFor(Container& container)
        {
            return [&container]()
            {
                return ArgumentResolver< std::shared_ptr< TArg > >::resolveFor< T >(container);
            };
        }
    };

} // namespace Traits
} // namespace Hypodermic