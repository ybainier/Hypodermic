#include <stdexcept>

#include "ISharingLifetimeScope.h"
#include "RootScopeLifetime.h"


namespace Hypodermic
{

    ISharingLifetimeScope* RootScopeLifetime::findScope(ISharingLifetimeScope* mostNestedVisibleScope)
    {
        if (mostNestedVisibleScope == nullptr)
            throw std::invalid_argument("mostNestedVisibleScope");
        return mostNestedVisibleScope->rootLifetimeScope();
    }

} // namespace Hypodermic
