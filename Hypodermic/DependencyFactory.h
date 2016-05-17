#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class Container;

    typedef std::function< std::shared_ptr< void >(Container&) > DependencyFactory;

} // namespace Hypodermic