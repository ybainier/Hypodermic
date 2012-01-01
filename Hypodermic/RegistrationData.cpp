#include "CurrentLifetimeScope.h"
#include "IComponentLifetime.h"
#include "RegistrationData.h"


namespace Hypodermic
{

    RegistrationData::RegistrationData(Service* defaultService)
        : defaultService_(defaultService)
        , defaultServiceOverriden_(false)
        , sharing_(InstanceSharing::None)
        , ownership_(InstanceOwnership::OwnedByLifetimeScope)
        , lifetime_(new CurrentLifetimeScope)
    {
    }

    InstanceSharing::Mode RegistrationData::sharing() const
    {
        return sharing_;
    }

    void RegistrationData::sharing( InstanceSharing::Mode value )
    {
        sharing_ = value;
    }

    InstanceOwnership::Mode RegistrationData::ownership() const
    {
        return ownership_;
    }

    void RegistrationData::ownership( InstanceOwnership::Mode value )
    {
        ownership_ = value;
    }

    IComponentLifetime* RegistrationData::lifetime()
    {
        return lifetime_;
    }

    void RegistrationData::lifetime(IComponentLifetime* value)
    {
        if (value == nullptr)
            std::invalid_argument("value");
        lifetime_ = value;
    }

    std::vector< Service* > RegistrationData::services()
    {
        if (defaultServiceOverriden_)
            return services_;

        return std::vector< Service* >(1, defaultService_);
    }

    void RegistrationData::addService(Service* service)
    {
        defaultServiceOverriden_ = true;
        services_.push_back(service);
    }

} // namespace Hypodermic