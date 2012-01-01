#ifndef     HYPODERMIC_INSTANCE_LOOKUP_H_
# define    HYPODERMIC_INSTANCE_LOOKUP_H_

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IInstanceLookup.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;
    class ILifetimeScope;
    class IResolveOperation;
    class ISharingLifetimeScope;


    class InstanceLookup : public IComponentContext, public IInstanceLookup
    {
    public:
        InstanceLookup(IComponentRegistration* registration, IResolveOperation* context,
                       ISharingLifetimeScope* mostNestedVisibleScope);

        void* execute();

        void complete();

        IComponentRegistry* componentRegistry();

        void* resolveComponent(IComponentRegistration* registration);

        IComponentRegistration* componentRegistration();

        ILifetimeScope* activationScope();

    private:
        bool newInstanceActivated();

        void* activate();

        IComponentRegistration* componentRegistration_;
        IResolveOperation* context_;
        ISharingLifetimeScope* activationScope_;
        void* newInstance_;
        bool executed_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_INSTANCE_LOOKUP_H_ */