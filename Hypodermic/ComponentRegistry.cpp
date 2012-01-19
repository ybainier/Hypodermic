#include <boost/foreach.hpp>

#include "IComponentRegistration.h"
#include "ComponentRegistry.h"


namespace Hypodermic
{

    IComponentRegistration* ComponentRegistry::getRegistration(Service* service)
    {
        boost::lock_guard< decltype (mutex_) > lock(mutex_);

        auto info = getInitializedServiceInfo(service);
        return info->getRegistration();
    }

    bool ComponentRegistry::isRegistered(Service* service)
    {
        return getInitializedServiceInfo(service)->isRegistered();
    }

    void ComponentRegistry::addRegistration(IComponentRegistration* registration)
    {
        addRegistration(registration, false);
    }

    void ComponentRegistry::addRegistration(IComponentRegistration* registration, bool preserveDefaults)
    {
        BOOST_FOREACH(Service* service, registration->services())
        {
            ServiceRegistrationInfo* info = getServiceInfo(service);
            info->addImplementation(registration);
        }
        registrations_.push_back(registration);
    }

    std::vector< IComponentRegistration* > ComponentRegistry::registrations()
    {
        return registrations_;
    }

    std::vector< IComponentRegistration* > ComponentRegistry::registrationsFor(Service* service)
    {
        boost::lock_guard< boost::recursive_mutex > lock(mutex_);

        auto info = getInitializedServiceInfo(service);
        return info->implementations();
    }

    ServiceRegistrationInfo* ComponentRegistry::getServiceInfo(Service* service)
    {
        std::type_index typeIndex(service->serviceTypeInfo());

        auto it = serviceInfo_.find(typeIndex);
        if (it != serviceInfo_.end())
            return it->second;

        ServiceRegistrationInfo* info = new ServiceRegistrationInfo(service);
        serviceInfo_.insert(ServiceRegistrationInfos::value_type(typeIndex, info));
        return info;
    }

    ServiceRegistrationInfo* ComponentRegistry::getInitializedServiceInfo(Service* service)
    {
        //TODO: retrieve actually initialized ServiceRegistrationInfo
        return getServiceInfo(service);
    }

} // namespace Hypodermic
