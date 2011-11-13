#include <boost/uuid/random_generator.hpp>
#include "LifetimeScope.h"


namespace Hypodermic
{
	boost::uuids::uuid LifetimeScope::selfRegistrationId_ = boost::uuids::random_generator()();
	Action< ContainerBuilder* > LifetimeScope::noConfiguration_ = [](ContainerBuilder*) -> void { };

} // namespace Hypodermic