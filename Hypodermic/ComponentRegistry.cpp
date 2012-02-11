#include <stdexcept>
#include <boost/foreach.hpp>

#include "IComponentRegistration.h"
#include "ComponentRegistry.h"


namespace Hypodermic
{

    std::shared_ptr< IComponentRegistration > ComponentRegistry::getRegistration(std::shared_ptr< Service > service)
    {
        boost::lock_guard< decltype (mutex_) > lock(mutex_);
        
        auto info = getServiceInfo(service);
        return info->getRegistration();
    }

    bool ComponentRegistry::isRegistered(std::shared_ptr< Service > service)
    {
        if (service == nullptr)
            throw std::invalid_argument("service");

        boost::lock_guard< decltype (mutex_) > lock(mutex_);
        return getServiceInfo(service)->isRegistered();
    }

    void ComponentRegistry::addRegistration(std::shared_ptr< IComponentRegistration > registration)
    {
        addRegistration(registration, false);
    }

    void ComponentRegistry::addRegistration(std::shared_ptr< IComponentRegistration > registration, bool /* preserveDefaults */)
    {
        boost::lock_guard< decltype (mutex_) > lock(mutex_);

        BOOST_FOREACH(auto service, registration->services())
        {
            auto info = getServiceInfo(service);
            info->addImplementation(registration);
        }
        registrations_.push_back(registration);
    }

    std::vector< std::shared_ptr< IComponentRegistration > > ComponentRegistry::registrations()
    {
        boost::lock_guard< decltype (mutex_) > lock(mutex_);
        return registrations_;
    }

    std::vector< std::shared_ptr< IComponentRegistration > > ComponentRegistry::registrationsFor(std::shared_ptr< Service > service)
    {
        boost::lock_guard< boost::recursive_mutex > lock(mutex_);

        auto info = getServiceInfo(service);
        return info->implementations();
    }

    std::shared_ptr< ServiceRegistrationInfo > ComponentRegistry::getServiceInfo(std::shared_ptr< Service > service)
    {
        ServiceKey key(service);

        auto it = serviceInfo_.find(key);
        if (it != serviceInfo_.end())
            return it->second;

        auto info = std::make_shared< ServiceRegistrationInfo >(service);
        serviceInfo_.insert(ServiceRegistrationInfos::value_type(key, info));
        return info;
    }

} // namespace Hypodermic
