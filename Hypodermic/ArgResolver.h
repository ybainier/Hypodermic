#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Hypodermic2/TypeInfo.h"


namespace Hypodermic2
{

    class Container;


namespace Traits
{

    template <class TArg>
    struct ArgResolver;


    template <class TArg>
    struct ArgResolver< TArg* >
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

        static std::string toString()
        {
            std::stringstream stream;

            stream << Utils::getMetaTypeInfo< TArg >().fullyQualifiedName() << "*";

            return stream.str();
        }
    };


    template <class TArg>
    struct ArgResolver< std::vector< TArg* > >
    {
        typedef std::vector< std::shared_ptr< TArg > > Type;

        template <class>
        static Type resolveFor(Container& container)
        {
            return container.resolveAll< TArg >();
        }

        static std::string toString()
        {
            std::stringstream stream;

            stream << "Vector< " << Utils::getMetaTypeInfo< TArg >().fullyQualifiedName() << "* >";

            return stream.str();
        }
    };


    template <class TArg>
    struct ArgResolver< std::function< TArg*() > >
    {
        typedef std::function< std::shared_ptr< TArg >() > Type;

        template <class T>
        static Type resolveFor(Container& container)
        {
            return [&container]()
            {
                return ArgResolver< TArg* >::resolveFor< T >(container);
            };
        }

        static std::string toString()
        {
            std::stringstream stream;

            stream << "Function< " << Utils::getMetaTypeInfo< TArg >().fullyQualifiedName() << "*() >";

            return stream.str();
        }
    };

} // namespace Traits
} // namespace Hypodermic2