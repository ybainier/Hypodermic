#ifndef		ICOMPONENT_REGISTRATION_H_
# define	ICOMPONENT_REGISTRATION_H_

# include <vector>
# include "IInstanceActivator.h"
# include "Service.h"


namespace Hypodermic
{

	class IComponentRegistration
	{
	public:
		virtual IInstanceActivator* activator() = 0;
		virtual std::vector< Service* >& services() = 0;
	};

} // namespace Hypodermic


#endif /* !ICOMPONENT_REGISTRATION_H_ */