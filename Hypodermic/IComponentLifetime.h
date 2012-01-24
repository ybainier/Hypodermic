#ifndef     HYPODERMIC_ICOMPONENT_LIFETIME_H_
# define    HYPODERMIC_ICOMPONENT_LIFETIME_H_


namespace Hypodermic
{

    class ISharingLifetimeScope;


    class IComponentLifetime
    {
    public:
        virtual ISharingLifetimeScope* findScope(ISharingLifetimeScope* mostNestedVisibleScope) = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_LIFETIME_H_ */