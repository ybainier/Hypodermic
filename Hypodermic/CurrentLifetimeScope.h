#ifndef     HYPODERMIC_CURRENT_LIFETIME_SCOPE_H_
# define    HYPODERMIC_CURRENT_LIFETIME_SCOPE_H_

# include <memory>
# include <Hypodermic/IComponentLifetime.h>


namespace Hypodermic
{
    class ISharingLifetimeScope;


    class CurrentLifetimeScope : public IComponentLifetime
    {
    public:
        std::shared_ptr< ISharingLifetimeScope > findScope(std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope);
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_CURRENT_LIFETIME_SCOPE_H_ */