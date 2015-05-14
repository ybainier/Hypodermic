#include "TypedService.h"
#include <functional>


namespace Hypodermic
{

    TypedService::TypedService(const std::type_info& typeInfo)
        : typeInfo_(typeInfo)
        , typeIndex_(typeInfo_)
    {
    }

    const std::type_info& TypedService::typeInfo() const
    {
        return typeInfo_;
    }

    bool TypedService::operator==(const Service& rhs) const
    {
        return typeInfo_ == rhs.typeInfo();
    }

    std::size_t TypedService::hashValue() const
    {
        return typeIndex_.hash_code();
    }

} // namespace Hypodermic