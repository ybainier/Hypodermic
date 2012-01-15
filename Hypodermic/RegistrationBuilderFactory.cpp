#include "ComponentRegistration.h"
#include "RegistrationData.h"
#include "RegistrationBuilderFactory.h"


namespace Hypodermic
{

    IComponentRegistration* RegistrationBuilderFactory::createRegistration(const boost::uuids::uuid& id,
                                                                           RegistrationData& registrationData,
                                                                           IInstanceActivator* activator,
			                                                               std::vector< Service* >& services,
                                                                           IComponentRegistration* target,
                                                                           const boost::unordered_map< std::type_index, ITypeCaster* >& typeCasters)
	{
        IComponentRegistration* registration;

        if (target == nullptr)
            registration = new ComponentRegistration(id,
                                                     activator,
                                                     registrationData.lifetime(),
                                                     registrationData.sharing(),
                                                     registrationData.ownership(),
                                                     services,
                                                     typeCasters);
        else
            registration = new ComponentRegistration(id,
                                                     activator,
                                                     registrationData.lifetime(),
                                                     registrationData.sharing(),
                                                     registrationData.ownership(),
                                                     services,
                                                     typeCasters,
                                                     target);

		return registration;
	}

} // namespace Hypodermic