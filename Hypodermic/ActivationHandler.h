#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class Container;

    typedef std::function< void(Container&, const std::shared_ptr< void >&) > ActivationHandler;

} // namespace Hypodermic