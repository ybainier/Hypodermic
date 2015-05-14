#include "Service.h"
#include "ServiceKey.h"


namespace Hypodermic
{

    ServiceKey::ServiceKey(std::shared_ptr< Service > service)
        : service_(service)
    {
    }

    bool ServiceKey::operator==(const ServiceKey& rhs) const
    {
        return *service_ == *rhs.service_;
    }


    std::size_t ServiceKey::hashValue() const
    {
        return service_->hashValue();
    }

} // namespace Hypodermic