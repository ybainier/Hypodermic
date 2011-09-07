#ifndef		COMPONENT_REGISTRATION_H_
# define	COMPONENT_REGISTRATION_H_

# include <vector>

# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class ComponentRegistration : public IComponentRegistration
	{
	public:
		ComponentRegistration(IInstanceActivator* activator, std::vector< Service* >& services)
			: activator_(activator), services_(services)
		{
		}

		IInstanceActivator* activator()
		{
			return activator_;
		}

		std::vector< Service* >& services()
		{
			return services_;
		}

	private:
		IInstanceActivator* activator_;
		std::vector< Service* > services_;
	};

} // namespace Hypodermic


#endif /* !COMPONENT_REGISTRATION_H_ */