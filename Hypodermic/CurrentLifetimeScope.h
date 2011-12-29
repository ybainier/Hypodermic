#ifndef     CURRENT_LIFETIME_SCOPE_H_
# define    CURRENT_LIFETIME_SCOPE_H_

# include <stdexcept>
# include <Hypodermic/IComponentLifetime.h>


namespace Hypodermic
{

    class CurrentLifetimeScope : public IComponentLifetime
    {
    public:
        ISharingLifetimeScope* findScope(ISharingLifetimeScope* mostNestedVisibleScope)
        {
            if (mostNestedVisibleScope == nullptr)
                throw std::invalid_argument("mostNestedVisibleScope");
            return mostNestedVisibleScope;
        }
    };

} // namespace Hypodermic


#endif /* !CURRENT_LIFETIME_SCOPE_H_ */