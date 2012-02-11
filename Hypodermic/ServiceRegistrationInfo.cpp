#include <stdexcept>

#include "NullptrWorkaround.h"
#include "Service.h"
#include "ServiceRegistrationInfo.h"


namespace Hypodermic
{

    ServiceRegistrationInfo::ServiceRegistrationInfo(std::shared_ptr< Service > service)
        : service_(service)
        , isInitialized_(false)
    {
        if (service == nullptr)
            throw std::invalid_argument("service");
    }

    std::vector< std::shared_ptr< IComponentRegistration > >& ServiceRegistrationInfo::implementations()
    {
        return implementations_;
    }

    void ServiceRegistrationInfo::addImplementation(std::shared_ptr< IComponentRegistration > registration)
    {
        implementations_.push_back(registration);
    }

    std::shared_ptr< IComponentRegistration > ServiceRegistrationInfo::getRegistration()
    {
        if (implementations_.size() > 0)
            return implementations_[implementations_.size() - 1];
        return std::shared_ptr< IComponentRegistration >();
    }

    bool ServiceRegistrationInfo::isRegistered()
    {
        return implementations_.size() > 0 && implementations_[0] != nullptr;
    }

} // namespace Hypodermic