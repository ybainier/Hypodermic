#ifndef     HYPODERMIC_CURRENT_LIFETIME_SCOPE_H_
# define    HYPODERMIC_CURRENT_LIFETIME_SCOPE_H_

# include <Hypodermic/IComponentLifetime.h>


namespace Hypodermic
{
    class ISharingLifetimeScope;


    class CurrentLifetimeScope : public IComponentLifetime
    {
    public:
        ISharingLifetimeScope* findScope(ISharingLifetimeScope* mostNestedVisibleScope);
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_CURRENT_LIFETIME_SCOPE_H_ */