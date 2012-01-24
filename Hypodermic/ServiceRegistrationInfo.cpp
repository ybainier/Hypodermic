#include "Service.h"
#include "ServiceRegistrationInfo.h"


namespace Hypodermic
{

    ServiceRegistrationInfo::ServiceRegistrationInfo(std::shared_ptr< Service > service)
        : service_(service)
        , isInitialized_(false)
    {
    }

    std::vector< IComponentRegistration* >& ServiceRegistrationInfo::implementations()
    {
        return implementations_;
    }

    void ServiceRegistrationInfo::addImplementation(IComponentRegistration* registration)
    {
        implementations_.push_back(registration);
    }

    IComponentRegistration* ServiceRegistrationInfo::getRegistration()
    {
        if (implementations_.size() > 0)
            return implementations_[0];
        return nullptr;
    }

    bool ServiceRegistrationInfo::isRegistered()
    {
        return implementations_.size() > 0 && implementations_[0] != nullptr;
    }

} // namespace Hypodermic