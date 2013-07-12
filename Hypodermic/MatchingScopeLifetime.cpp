#include "MatchingScopeLifetime.h"

#include <stdexcept>

#include "ISharingLifetimeScope.h"


namespace Hypodermic
{

    std::shared_ptr< ISharingLifetimeScope > MatchingScopeLifetime::findScope(std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope)
    {
        if (mostNestedVisibleScope == nullptr)
            throw std::invalid_argument("mostNestedVisibleScope");

        // TODO: lookup by tag eventually...

        return mostNestedVisibleScope;
    }

} // namespace Hypodermic
