#ifndef		ICOMPONENT_REGISTRY_H_
# define	ICOMPONENT_REGISTRY_H_

# include <vector>

# include "Service.h"


namespace Hypodermic
{
	class IComponentRegistration;


	class IComponentRegistry
	{
	public:
		virtual IComponentRegistration* getRegistration(Service* service) = 0;
		
		virtual bool isRegistered(Service* service) = 0;
		
		virtual void addRegistration(IComponentRegistration* registration) = 0;

		virtual std::vector< IComponentRegistration* > registrations() = 0;

		virtual std::vector< IComponentRegistration* > registrationsFor(Service* service) = 0;
	};

} // namespace Hypodermic


#endif /* !ICOMPONENT_REGISTRY_H_ */