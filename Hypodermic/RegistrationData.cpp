#include <stdexcept>

#include "CurrentLifetimeScope.h"
#include "IComponentLifetime.h"
#include "NullptrWorkaround.h"
#include "RegistrationData.h"


namespace Hypodermic
{

    RegistrationData::RegistrationData(std::shared_ptr< Service > defaultService)
        : defaultService_(defaultService)
        , defaultServiceOverriden_(false)
        , sharing_(InstanceSharing::None)
        , ownership_(InstanceOwnership::OwnedByLifetimeScope)
        , lifetime_(new CurrentLifetimeScope)
        , services_()
        , preparingCallbacks_()
        , activatingCallbacks_()
        , activatedCallbacks_()
    {
    }

    InstanceSharing::Mode RegistrationData::sharing() const
    {
        return sharing_;
    }

    void RegistrationData::sharing(InstanceSharing::Mode value)
    {
        sharing_ = value;
    }

    InstanceOwnership::Mode RegistrationData::ownership() const
    {
        return ownership_;
    }

    void RegistrationData::ownership(InstanceOwnership::Mode value)
    {
        ownership_ = value;
    }

    std::shared_ptr< IComponentLifetime > RegistrationData::lifetime()
    {
        return lifetime_;
    }

    void RegistrationData::lifetime(std::shared_ptr< IComponentLifetime > value)
    {
        if (value == nullptr)
            std::invalid_argument("value");
        lifetime_ = value;
    }

    std::vector< std::shared_ptr< Service > > RegistrationData::services()
    {
        if (defaultServiceOverriden_)
            return services_;

        return std::vector< std::shared_ptr< Service > >(1, defaultService_);
    }

    void RegistrationData::addService(std::shared_ptr< Service > service)
    {
        defaultServiceOverriden_ = true;
        services_.push_back(service);
    }

    RegistrationData::PreparingCallbacks& RegistrationData::preparingCallbacks()
    {
        return preparingCallbacks_;
    }

    RegistrationData::ActivatingCallbacks& RegistrationData::activatingCallbacks()
    {
        return activatingCallbacks_;
    }

    RegistrationData::ActivatedCallbacks& RegistrationData::activatedCallbacks()
    {
        return activatedCallbacks_;
    }

} // namespace Hypodermic