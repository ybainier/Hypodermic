#ifndef     HYPODERMIC_MATCHING_SCOPE_LIFETIME_H_
# define    HYPODERMIC_MATCHING_SCOPE_LIFETIME_H_

# include <memory>
# include <Hypodermic/IComponentLifetime.h>


namespace Hypodermic
{
    class ISharingLifetimeScope;


    class MatchingScopeLifetime : public IComponentLifetime
    {
    public:
        std::shared_ptr< ISharingLifetimeScope > findScope(std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope);
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_MATCHING_SCOPE_LIFETIME_H_ */