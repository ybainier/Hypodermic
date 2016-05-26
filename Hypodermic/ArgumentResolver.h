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

        template <class T, class TRegistration, class TComponentContext>
        static Type resolveFor(const TRegistration& registration, TComponentContext& context)
        {
            auto&& factory = context.template getDependencyFactory< T, TArg >(registration);
            if (!factory)
                return context.template resolve< TArg >();

            return factory(context);
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::vector< std::shared_ptr< TArg > > >
    {
        typedef std::vector< std::shared_ptr< TArg > > Type;

        template <class, class TRegistration, class TComponentContext>
        static Type resolveFor(const TRegistration&, TComponentContext& context)
        {
            return context.template resolveAll< TArg >();
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::function< std::shared_ptr< TArg >() > >
    {
        typedef std::function< std::shared_ptr< TArg >() > Type;

        template <class T, class TRegistration, class TComponentContext>
        static Type resolveFor(const TRegistration& registration, TComponentContext& context)
        {
            return [&registration, &context]()
            {
                return ArgumentResolver< std::shared_ptr< TArg > >::template resolveFor< T >(registration, context);
            };
        }
    };

} // namespace Traits
} // namespace Hypodermic