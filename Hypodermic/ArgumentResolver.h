#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Hypodermic/FactoryWrapper.h"
#include "Hypodermic/IsComplete.h"
#include "Hypodermic/ResolutionException.h"
#include "Hypodermic/TypeInfo.h"


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

        template <class TRegistration, class TResolutionContext>
        static Type resolve(const TRegistration& registration, TResolutionContext& resolutionContext)
        {
            static_assert(IsComplete< TArg >::value, "TArg should be a complete type");

            auto&& factory = registration.getDependencyFactory(Utils::getMetaTypeInfo< TArg >());
            if (factory)
                return std::static_pointer_cast< TArg >(factory(resolutionContext.componentContext()));

            return resolutionContext.componentContext().template resolve< TArg >();
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::vector< std::shared_ptr< TArg > > >
    {
        typedef std::vector< std::shared_ptr< TArg > > Type;

        template <class TRegistration, class TResolutionContext>
        static Type resolve(const TRegistration&, TResolutionContext& resolutionContext)
        {
            return resolutionContext.componentContext().template resolveAll< TArg >();
        }
    };


    template <class TArg>
    struct ArgumentResolver< std::function< std::shared_ptr< TArg >() > >
    {
        typedef std::function< std::shared_ptr< TArg >() > Type;

        template <class TRegistration, class TResolutionContext>
        static Type resolve(const TRegistration&, TResolutionContext& resolutionContext)
        {
            auto&& weakContainer = resolutionContext.container();

            return [weakContainer]() -> std::shared_ptr< TArg >
            {
                auto&& container = weakContainer.lock();
                if (container == nullptr)
                    HYPODERMIC_THROW_RESOLUTION_EXCEPTION("The container is not available to resolve " << Utils::getMetaTypeInfo< TArg >().fullyQualifiedName());

                return container->template resolve< TArg >();
            };
        }
    };


    template <class TArg>
    struct ArgumentResolver< FactoryWrapper< TArg > >
    {
        typedef FactoryWrapper< TArg > Type;

        template <class TRegistration, class TResolutionContext>
        static Type resolve(const TRegistration& registration, TResolutionContext& resolutionContext)
        {
            return Type(ArgumentResolver< std::function< std::shared_ptr< TArg >() > >::template resolve(registration, resolutionContext));
        }
    };

} // namespace Traits
} // namespace Hypodermic