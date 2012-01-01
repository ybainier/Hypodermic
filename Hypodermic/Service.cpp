#include "Service.h"


namespace Hypodermic
{

    bool Service::operator!=(const Service& rhs) const
    {
        return !(*this == rhs);
    }

} // namespace Hypodermic
