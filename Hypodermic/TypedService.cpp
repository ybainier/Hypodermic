#include "TypedService.h"


namespace Hypodermic
{

    TypedService::TypedService(const std::type_info& typeInfo)
        : typeInfo_(typeInfo)
    {
    }

    const std::type_info& TypedService::typeInfo() const
    {
        return typeInfo_;
    }

} // namespace Hypodermic