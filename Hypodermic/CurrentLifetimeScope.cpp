#include <stdexcept>

#include "ISharingLifetimeScope.h"
#include "CurrentLifetimeScope.h"


namespace Hypodermic
{

    ISharingLifetimeScope* CurrentLifetimeScope::findScope(ISharingLifetimeScope* mostNestedVisibleScope)
    {
        if (mostNestedVisibleScope == nullptr)
            throw std::invalid_argument("mostNestedVisibleScope");
        return mostNestedVisibleScope;
    }

} // namespace Hypodermic
