#ifndef     ROOT_SCOPE_LIFETIME_H_
# define    ROOT_SCOPE_LIFETIME_H_

# include <stdexcept>
# include <Hypodermic/IComponentLifetime.h>


namespace Hypodermic
{

    class RootScopeLifetime : public IComponentLifetime
    {
    public:
        ISharingLifetimeScope* findScope(ISharingLifetimeScope* mostNestedVisibleScope)
        {
            if (mostNestedVisibleScope == nullptr)
                throw std::invalid_argument("mostNestedVisibleScope");
            return mostNestedVisibleScope->rootLifetimeScope();
        }
    };

} // namespace Hypodermic


#endif /* !ROOT_SCOPE_LIFETIME_H_ */