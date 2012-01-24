#include <stdexcept>

#include "ISharingLifetimeScope.h"
#include "CurrentLifetimeScope.h"


namespace Hypodermic
{

    std::shared_ptr< ISharingLifetimeScope > CurrentLifetimeScope::findScope(std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope)
    {
        if (mostNestedVisibleScope == nullptr)
            throw std::invalid_argument("mostNestedVisibleScope");
        return mostNestedVisibleScope;
    }

} // namespace Hypodermic
