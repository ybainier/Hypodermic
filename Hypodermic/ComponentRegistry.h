#ifndef		HYPODERMIC_COMPONENT_REGISTRY_H_
# define	HYPODERMIC_COMPONENT_REGISTRY_H_

# include <vector>
# include <boost/foreach.hpp>
# include <boost/unordered_map.hpp>
# include <boost/thread.hpp>

# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/ServiceRegistrationInfo.h>
# include <Hypodermic/ServiceKey.h>


namespace Hypodermic
{

	class ComponentRegistry : public IComponentRegistry
	{
		typedef boost::unordered_map< ServiceKey, ServiceRegistrationInfo* > ServiceInfoByServiceKey;

	public:
		IComponentRegistration* getRegistration(Service* service)
		{
			boost::lock_guard< boost::recursive_mutex > lock(mutex_);

			auto info = getInitializedServiceInfo(service);
			return info->getRegistration();
		}

		bool isRegistered(Service* service)
		{
			return getInitializedServiceInfo(service)->isRegistered();
		}

		void addRegistration(IComponentRegistration* registration)
		{
			registrations_.push_back(registration);

			BOOST_FOREACH(Service* service, registration->services())
			{
				ServiceRegistrationInfo* info = getServiceInfo(service);
				info->addImplementation(registration);
			}
		}

		std::vector< IComponentRegistration* > registrations()
		{
			return registrations_;
		}

		std::vector< IComponentRegistration* > registrationsFor(Service* service)
		{
			boost::lock_guard< boost::recursive_mutex > lock(mutex_);

			auto info = getInitializedServiceInfo(service);
			return info->implementations();
		}

		ServiceRegistrationInfo* getServiceInfo(Service* service)
		{
			ServiceInfoByServiceKey::iterator iServiceInfoByServiceKey = serviceInfo_.find(ServiceKey(service));
			if (iServiceInfoByServiceKey != serviceInfo_.end())
				return iServiceInfoByServiceKey->second;

			ServiceRegistrationInfo* info = new ServiceRegistrationInfo(service);
			serviceInfo_.insert(ServiceInfoByServiceKey::value_type(ServiceKey(service), info));
			return info;
		}

		ServiceRegistrationInfo* getInitializedServiceInfo(Service* service)
		{
			//TODO: retrieve actually initialized ServiceRegistrationInfo
			return getServiceInfo(service);
		}

	private:
		std::vector< IComponentRegistration* > registrations_;
		ServiceInfoByServiceKey serviceInfo_;
		boost::recursive_mutex mutex_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COMPONENT_REGISTRY_H_ */