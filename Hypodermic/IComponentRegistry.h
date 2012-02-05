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
        virtual ~IComponentRegistry() {}

		virtual std::shared_ptr< IComponentRegistration > getRegistration(std::shared_ptr< Service > service) = 0;
		
		virtual bool isRegistered(std::shared_ptr< Service > service) = 0;
		
		virtual void addRegistration(std::shared_ptr< IComponentRegistration > registration) = 0;
        virtual void addRegistration(std::shared_ptr< IComponentRegistration > registration, bool preserveDefaults) = 0;

		virtual std::vector< std::shared_ptr< IComponentRegistration > > registrations() = 0;

		virtual std::vector< std::shared_ptr< IComponentRegistration > > registrationsFor(std::shared_ptr< Service > service) = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_REGISTRY_H_ */