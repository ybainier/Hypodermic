#include "ComponentRegistration.h"
#include "RegistrationData.h"
#include "RegistrationBuilderFactory.h"


namespace Hypodermic
{

    std::shared_ptr< IComponentRegistration > RegistrationBuilderFactory::createRegistration(const boost::uuids::uuid& id,
                                                                                             RegistrationData& registrationData,
                                                                                             std::shared_ptr< IInstanceActivator > activator,
			                                                                                 std::vector< std::shared_ptr< Service > > services,
                                                                                             std::shared_ptr< IComponentRegistration > target,
                                                                                             const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >& typeCasters)
	{
        std::shared_ptr< IComponentRegistration > registration;

        if (target == nullptr)
            registration = std::make_shared< ComponentRegistration >(id, activator, registrationData.lifetime(), registrationData.sharing(),
                                                                     registrationData.ownership(), services, typeCasters);
        else
            registration = std::make_shared< ComponentRegistration >(id, activator, registrationData.lifetime(), registrationData.sharing(),
                                                                     registrationData.ownership(), services, typeCasters, target);

		return registration;
	}

} // namespace Hypodermic