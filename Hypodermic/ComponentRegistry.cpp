#include "ComponentRegistry.h"

#include <algorithm>
#include <stdexcept>
#include <boost/foreach.hpp>

#include "IComponentRegistration.h"
#include "IRegistrationSource.h"
#include "NullptrWorkaround.h"


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
        if (service == nullptr)
            throw std::invalid_argument("service");

        boost::lock_guard< decltype (mutex_) > lock(mutex_);
        return getInitializedServiceInfo(service)->isRegistered();
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

        auto info = getInitializedServiceInfo(service);
        return info->implementations();
    }

    std::shared_ptr< ServiceRegistrationInfo > ComponentRegistry::getInitializedServiceInfo(std::shared_ptr< Service > service)
    {
        auto info = getServiceInfo(service);
        if (info->isInitialized())
            return info;

        if (!info->isInitializing())
            info->beginInitialization(dynamicRegistrationSources_);

        while (info->hasSourcesToQuery())
        {
            auto next = info->dequeueNextSource();
            auto&& registrations = next->registrationsFor(service, [this](std::shared_ptr< Service > service) { return this->registrationsFor(service); });

            std::for_each(registrations.begin(), registrations.end(),
                [&](std::shared_ptr< IComponentRegistration > provided)
                {
                    auto self = this;

                    auto&& services = provided->services();
                    std::for_each(services.begin(), services.end(),
                        [&](std::shared_ptr< Service > additionalService)
                        {
                            auto additionalInfo = self->getServiceInfo(additionalService);
                            if (additionalInfo->isInitialized())
                                return;

                            if (!additionalInfo->isInitializing())
                            {
                                decltype(self->dynamicRegistrationSources_) registrationSources;

                                std::copy_if(self->dynamicRegistrationSources_.begin(), self->dynamicRegistrationSources_.end(),
                                             std::back_inserter(registrationSources),
                                             [&](std::shared_ptr< IRegistrationSource > src) { return src != next; });

                                additionalInfo->beginInitialization(registrationSources);
                            }
                            else
                                additionalInfo->skipSource(next);
                        });

                    this->addRegistration(provided, true);
                });
        }

        info->completeInitialization();
        return info;
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

    bool ComponentRegistry::hasLocalComponents() const
    {
        return true;
    }

    void ComponentRegistry::addRegistrationSource(std::shared_ptr< IRegistrationSource > source)
    {
        if (source == nullptr)
            throw std::invalid_argument("source");

        boost::lock_guard< boost::recursive_mutex > lock(mutex_);

        dynamicRegistrationSources_.push_front(source);

        std::for_each(serviceInfo_.begin(), serviceInfo_.end(),
            [source](ServiceRegistrationInfos::value_type& pair)
            {
                pair.second->include(source);
            });
    }

} // namespace Hypodermic
