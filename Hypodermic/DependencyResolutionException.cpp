#include "DependencyResolutionException.h"


namespace Hypodermic
{

    DependencyResolutionException::DependencyResolutionException(std::string message)
        : std::logic_error(message)
    {
    }

} // namespace Hypodermic