#ifndef     HYPODERMIC_ICOMPONENT_LIFETIME_H_
# define    HYPODERMIC_ICOMPONENT_LIFETIME_H_

# include <memory>


namespace Hypodermic
{

    class ISharingLifetimeScope;


    class IComponentLifetime
    {
    public:
        virtual ~IComponentLifetime() {}

        virtual std::shared_ptr< ISharingLifetimeScope > findScope(std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope) = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_LIFETIME_H_ */