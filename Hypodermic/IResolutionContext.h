#pragma once

#include <memory>


namespace Hypodermic
{

    class ComponentContext;
    class Container;

    
    class IResolutionContext
    {
    public:
        virtual ~IResolutionContext() = default;

        virtual ComponentContext& componentContext() const = 0;

        virtual const std::weak_ptr< Container >& container() const = 0;
    };

} // namespace Hypodermic