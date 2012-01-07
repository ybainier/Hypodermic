#include <boost/foreach.hpp>
#include "ContainerBuilder.h"


namespace Hypodermic
{

    boost::shared_ptr< IContainer > ContainerBuilder::build()
    {
        boost::shared_ptr< IContainer > container = new Container;

        BOOST_FOREACH(auto callback, containerConfigurationCallbacks_)
        {
            callback(*container);
        }

        return container;
    }

} // namespace Hypodermic