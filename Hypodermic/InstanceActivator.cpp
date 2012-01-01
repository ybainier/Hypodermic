#include "InstanceActivator.h"


namespace Hypodermic
{

    InstanceActivator::InstanceActivator(const std::type_info& typeInfo)
        : typeInfo_(typeInfo)
    {
    }

    const std::type_info& InstanceActivator::typeInfo()
    {
        return typeInfo_;
    }

} // Hypodermic
