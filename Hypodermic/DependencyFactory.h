#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class ComponentContext;

    typedef std::function< std::shared_ptr< void >(ComponentContext&) > DependencyFactory;

} // namespace Hypodermic