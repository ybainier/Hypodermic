#include <boost/foreach.hpp>

#include "IComponentRegistration.h"
#include "ComponentRegistry.h"


namespace Hypodermic
{

    std::shared_ptr< IComponentRegistration > ComponentRegistry::getRegistration(std::shared_ptr< Service > service)
    {
        boost::lock_guard< decltype (mutex_) > lock(mutex_);

        auto info = getInitializedServiceInfo(service);
        return info->getRegistration();
    }

    bool ComponentRegistry::isRegistered(std::shared_ptr< Service > service)
    {
        return getInitializedServiceInfo(service)->isRegistered();
    }

    void ComponentRegistry::addRegistration(std::shared_ptr< IComponentRegistration > registration)
    {
        addRegistration(registration, false);
    }

    void ComponentRegistry::addRegistration(std::shared_ptr< IComponentRegistration > registration, bool preserveDefaults)
    {
        BOOST_FOREACH(auto service, registration->services())
        {
            auto info = getServiceInfo(service);
            info->addImplementation(registration);
        }
        registrations_.push_back(registration);
    }

    std::vector< std::shared_ptr< IComponentRegistration > > ComponentRegistry::registrations()
    {
        return registrations_;
    }

    std::vector< std::shared_ptr< IComponentRegistration > > ComponentRegistry::registrationsFor(std::shared_ptr< Service > service)
    {
        boost::lock_guard< boost::recursive_mutex > lock(mutex_);

        auto info = getInitializedServiceInfo(service);
        return info->implementations();
    }

    std::shared_ptr< ServiceRegistrationInfo > ComponentRegistry::getServiceInfo(std::shared_ptr< Service > service)
    {
        std::type_index typeIndex(service->typeInfo());

        auto it = serviceInfo_.find(typeIndex);
        if (it != serviceInfo_.end())
            return it->second;

        auto info = std::make_shared< ServiceRegistrationInfo >(service);
        serviceInfo_.insert(ServiceRegistrationInfos::value_type(typeIndex, info));
        return info;
    }

    std::shared_ptr< ServiceRegistrationInfo > ComponentRegistry::getInitializedServiceInfo(std::shared_ptr< Service > service)
    {
        //TODO: retrieve actually initialized ServiceRegistrationInfo
        return getServiceInfo(service);
    }

} // namespace Hypodermic
