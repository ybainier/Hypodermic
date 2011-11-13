#ifndef		HYPODERMIC_ICOMPONENT_REGISTRATION_H_
# define	HYPODERMIC_ICOMPONENT_REGISTRATION_H_

# include <vector>

# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class IComponentRegistration
	{
	public:
		virtual IInstanceActivator* activator() = 0;
		virtual std::vector< Service* >& services() = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_REGISTRATION_H_ */