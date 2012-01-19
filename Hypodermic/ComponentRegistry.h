#ifndef		HYPODERMIC_COMPONENT_REGISTRY_H_
# define	HYPODERMIC_COMPONENT_REGISTRY_H_

# include <typeindex>
# include <vector>

# include <boost/unordered_map.hpp>
# include <boost/thread.hpp>

# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/ServiceRegistrationInfo.h>


namespace Hypodermic
{
    class IComponentRegistration;


	class ComponentRegistry : public IComponentRegistry
	{
		typedef boost::unordered_map< std::type_index, ServiceRegistrationInfo* > ServiceRegistrationInfos;

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
		ServiceRegistrationInfos serviceInfo_;
		boost::recursive_mutex mutex_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COMPONENT_REGISTRY_H_ */