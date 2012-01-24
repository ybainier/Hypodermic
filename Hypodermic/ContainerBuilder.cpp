#include <boost/foreach.hpp>

#include "Container.h"
#include "ContainerBuilder.h"


namespace Hypodermic
{

    ContainerBuilder::ContainerBuilder()
    {
        wasBuilt_ = false;
    }

    void ContainerBuilder::registerCallback(ConfigurationCallback configurationCallback)
    {
        configurationCallbacks_.push_back(configurationCallback);
    }

    std::shared_ptr< IContainer > ContainerBuilder::build()
    {
        auto result = std::make_shared< Container >();
        result->initialize();

        build(result->componentRegistry());

        return result;
    }

    void ContainerBuilder::build(std::shared_ptr< IComponentRegistry > componentRegistry)
    {
        if (componentRegistry == nullptr)
            throw std::invalid_argument("componentRegistry");

        if (wasBuilt_)
            throw std::logic_error("ContainerBuilder::build() can only be called once");

        wasBuilt_ = true;

        BOOST_FOREACH(auto callback, configurationCallbacks_)
            callback(componentRegistry);
    }

} // namespace Hypodermic