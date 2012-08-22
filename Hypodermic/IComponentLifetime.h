#ifndef     HYPODERMIC_ICOMPONENT_LIFETIME_H_
# define    HYPODERMIC_ICOMPONENT_LIFETIME_H_

# include <memory>


namespace Hypodermic
{

    class ISharingLifetimeScope;


    class IComponentLifetime
    {
    public:
        virtual ~IComponentLifetime() = 0;

        virtual std::shared_ptr< ISharingLifetimeScope > findScope(std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope) = 0;
    };

    inline IComponentLifetime::~IComponentLifetime() {}

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_LIFETIME_H_ */