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

    Container* ContainerBuilder::build()
    {
        auto result = new Container;
        build(result->componentRegistry());

        return result;
    }

    void ContainerBuilder::build(IComponentRegistry* componentRegistry)
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