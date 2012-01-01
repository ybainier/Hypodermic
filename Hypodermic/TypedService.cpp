#include <boost/functional/hash.hpp>
#include "TypedService.h"


namespace Hypodermic
{

    TypedService::TypedService(const type_info& serviceTypeInfo)
        : serviceTypeInfo_(serviceTypeInfo)
    {
    }

    const type_info& TypedService::serviceTypeInfo() const
    {
        return serviceTypeInfo_;
    }

    bool TypedService::operator==(const Service& rhs) const
    {
        const TypedService* that = static_cast< const TypedService* >(&rhs);

        if (that == nullptr)
            return false;

        return serviceTypeInfo_ == that->serviceTypeInfo_;
    }

    std::size_t TypedService::hashValue()
    {
        return boost::hash< const char* >()(serviceTypeInfo_.name());
    }

} // namespace Hypodermic