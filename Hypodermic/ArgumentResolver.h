#pragma once

#include <functional>
#include <memory>
#include <vector>


namespace Hypodermic
{
namespace Traits
{

    template <class TArg>
    struct ArgumentResolver;


    template <class TArg>
    struct ArgumentResolver< std::shared_ptr< TArg > >
    {
        typedef std::shared_ptr< TArg > Type;

        template <class T, class TContainer>
        static Type resolveFor(TContainer& container)
        {
            auto&& factory = container.template getDependencyFactory< T, TArg >();
            if (!factory)
                return container.template resolve< TArg >();

            return factory(container);
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::vector< std::shared_ptr< TArg > > >
    {
        typedef std::vector< std::shared_ptr< TArg > > Type;

        template <class, class TContainer>
        static Type resolveFor(TContainer& container)
        {
            return container.template resolveAll< TArg >();
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::function< std::shared_ptr< TArg >() > >
    {
        typedef std::function< std::shared_ptr< TArg >() > Type;

        template <class T, class TContainer>
        static Type resolveFor(TContainer& container)
        {
            return [&container]()
            {
                return ArgumentResolver< std::shared_ptr< TArg > >::template resolveFor< T >(container);
            };
        }
    };

} // namespace Traits
} // namespace Hypodermic