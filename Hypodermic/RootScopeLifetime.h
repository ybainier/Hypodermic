#ifndef     HYPODERMIC_ROOT_SCOPE_LIFETIME_H_
# define    HYPODERMIC_ROOT_SCOPE_LIFETIME_H_

# include <Hypodermic/IComponentLifetime.h>


namespace Hypodermic
{
    class ISharingLifetimeScope;


    class RootScopeLifetime : public IComponentLifetime
    {
    public:
        std::shared_ptr< ISharingLifetimeScope > findScope(std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope);
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_ROOT_SCOPE_LIFETIME_H_ */