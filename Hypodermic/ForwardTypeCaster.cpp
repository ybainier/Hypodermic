#include "ForwardTypeCaster.h"


namespace Hypodermic
{

    ForwardTypeCaster::ForwardTypeCaster(const std::type_info& typeInfo)
        : typeInfo_(typeInfo)
    {
    }

    const std::type_info& ForwardTypeCaster::typeInfo() const
    {
        return typeInfo_;
    }

    std::shared_ptr< void > ForwardTypeCaster::cast(std::shared_ptr< void > concreteInstance) const
    {
        return concreteInstance;
    }

} // namespace Hypodermic

