#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class ComponentContext;

    typedef std::function< void(ComponentContext&, const std::shared_ptr< void >&) > ActivationHandler;

} // namespace Hypodermic