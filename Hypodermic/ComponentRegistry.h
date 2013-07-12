#ifndef		HYPODERMIC_COMPONENT_REGISTRY_H_
# define	HYPODERMIC_COMPONENT_REGISTRY_H_

# include <deque>
# include <memory>
# include <unordered_map>
# include <vector>

# include <boost/thread.hpp>

# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/ServiceKey.h>
# include <Hypodermic/ServiceRegistrationInfo.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IRegistrationSource;


	class ComponentRegistry : public IComponentRegistry
	{
		typedef std::unordered_map< ServiceKey, std::shared_ptr< ServiceRegistrationInfo > > ServiceRegistrationInfos;

	public:
		std::shared_ptr< IComponentRegistration > getRegistration(std::shared_ptr< Service > service);

		bool isRegistered(std::shared_ptr< Service > service);

		void addRegistration(std::shared_ptr< IComponentRegistration > registration);

        virtual void addRegistration(std::shared_ptr< IComponentRegistration > registration, bool preserveDefaults);

        void addRegistrationSource(std::shared_ptr< IRegistrationSource > source);

		std::vector< std::shared_ptr< IComponentRegistration > > registrations();

		std::vector< std::shared_ptr< IComponentRegistration > > registrationsFor(std::shared_ptr< Service > service);

        bool hasLocalComponents() const;

	private:
        std::shared_ptr< ServiceRegistrationInfo > getInitializedServiceInfo(std::shared_ptr< Service > service);
		std::shared_ptr< ServiceRegistrationInfo > getServiceInfo(std::shared_ptr< Service > service);
        
        std::vector< std::shared_ptr< IComponentRegistration > > registrations_;
        std::deque< std::shared_ptr< IRegistrationSource > > dynamicRegistrationSources_;
		ServiceRegistrationInfos serviceInfo_;
		boost::recursive_mutex mutex_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COMPONENT_REGISTRY_H_ */