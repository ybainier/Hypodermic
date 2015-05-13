#include <stdexcept>
#include "ContainerActivator.h"


namespace Hypodermic
{
    ContainerActivator::ContainerActivator(const std::shared_ptr< IContainer >& instance)
        : InstanceActivator(typeid(IContainer))
        , instance_(instance)
    {
    }

    std::shared_ptr< void > ContainerActivator::activateInstance(const std::shared_ptr< IComponentContext >&)
    {
        return instance_.lock();
    }

} // namespace Hypodermic
