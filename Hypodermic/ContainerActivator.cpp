#include <stdexcept>
#include "ContainerActivator.h"


namespace Hypodermic
{
    ContainerActivator::ContainerActivator(std::shared_ptr< IContainer > instance)
        : InstanceActivator(typeid(IContainer)), instance_(instance)
    {
    }

    std::shared_ptr< void > ContainerActivator::activateInstance(std::shared_ptr< IComponentContext > /* context */)
    {
        return instance_.lock();
    }

} // namespace Hypodermic
