#ifndef		HYPODERMIC_ICOMPONENT_REGISTRY_H_
# define	HYPODERMIC_ICOMPONENT_REGISTRY_H_

# include <memory>
# include <vector>

# include <Hypodermic/Service.h>


namespace Hypodermic
{
	class IComponentRegistration;


	class IComponentRegistry
	{
	public:
		virtual IComponentRegistration* getRegistration(std::shared_ptr< Service > service) = 0;
		
		virtual bool isRegistered(std::shared_ptr< Service > service) = 0;
		
		virtual void addRegistration(IComponentRegistration* registration) = 0;
        virtual void addRegistration(IComponentRegistration* registration, bool preserveDefaults) = 0;

		virtual std::vector< IComponentRegistration* > registrations() = 0;

		virtual std::vector< IComponentRegistration* > registrationsFor(std::shared_ptr< Service > service) = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_REGISTRY_H_ */