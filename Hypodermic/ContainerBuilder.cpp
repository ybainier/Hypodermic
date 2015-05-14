#include <boost/foreach.hpp>

#include "Container.h"
#include "ContainerBuilder.h"


namespace Hypodermic
{

    ContainerBuilder::ContainerBuilder()
    {
        wasBuilt_ = false;
    }

    void ContainerBuilder::registerCallback(const ConfigurationCallback& configurationCallback)
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

    void ContainerBuilder::build(const std::shared_ptr< IComponentRegistry >& componentRegistry)
    {
        if (componentRegistry == nullptr)
            throw std::invalid_argument("componentRegistry");

        if (wasBuilt_)
            throw std::logic_error("ContainerBuilder::build() can only be called once");

        wasBuilt_ = true;

        BOOST_FOREACH(auto callback, configurationCallbacks_)
            callback(componentRegistry);
    }

    void ContainerBuilder::registerSource(const std::shared_ptr< IRegistrationSource >& registrationSource)
    {
        if (registrationSource == nullptr)
            throw std::invalid_argument("registrationSource");

        registerCallback([registrationSource](const std::shared_ptr< IComponentRegistry >& cr)
        {
            cr->addRegistrationSource(registrationSource);
        });
    }

    void ContainerBuilder::addRegistrations(const ContainerBuilder& containerBuilder)
    {
        if (wasBuilt_)
            throw std::logic_error("ContainerBuilder::addRegistrations() cannot be invoked on a built container");

        BOOST_FOREACH(auto callback, containerBuilder.configurationCallbacks_)
            configurationCallbacks_.push_back(callback);
    }

} // namespace Hypodermic