#ifndef		HYPODERMIC_COMPONENT_REGISTRY_H_
# define	HYPODERMIC_COMPONENT_REGISTRY_H_

# include <vector>
# include <boost/unordered_map.hpp>
# include <boost/thread.hpp>

# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/ServiceRegistrationInfo.h>
# include <Hypodermic/ServiceKey.h>


namespace Hypodermic
{
    class IComponentRegistration;


	class ComponentRegistry : public IComponentRegistry
	{
		typedef boost::unordered_map< ServiceKey, ServiceRegistrationInfo* > ServiceInfoByServiceKey;

	public:
		IComponentRegistration* getRegistration(Service* service);

		bool isRegistered(Service* service);

		void addRegistration(IComponentRegistration* registration);

        void addRegistration(IComponentRegistration* registration, bool preserveDefaults);

		std::vector< IComponentRegistration* > registrations();

		std::vector< IComponentRegistration* > registrationsFor(Service* service);

		ServiceRegistrationInfo* getServiceInfo(Service* service);

		ServiceRegistrationInfo* getInitializedServiceInfo(Service* service);

	private:
		std::vector< IComponentRegistration* > registrations_;
		ServiceInfoByServiceKey serviceInfo_;
		boost::recursive_mutex mutex_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COMPONENT_REGISTRY_H_ */