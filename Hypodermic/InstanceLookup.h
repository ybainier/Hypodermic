#ifndef     HYPODERMIC_INSTANCE_LOOKUP_H_
# define    HYPODERMIC_INSTANCE_LOOKUP_H_

# include <memory>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IInstanceLookup.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;
    class ILifetimeScope;
    class IResolveOperation;
    class ISharingLifetimeScope;


    class InstanceLookup : public std::enable_shared_from_this< InstanceLookup >, public IComponentContext, public IInstanceLookup
    {
    public:
        InstanceLookup(std::shared_ptr< IComponentRegistration > registration, std::shared_ptr< IResolveOperation > context,
                       std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope);

        std::shared_ptr< void > execute();

        void complete();

        std::shared_ptr< IComponentRegistry > componentRegistry();

        std::shared_ptr< void > resolveComponent(std::shared_ptr< IComponentRegistration > registration);

        std::shared_ptr< IComponentRegistration > componentRegistration();

        std::shared_ptr< ILifetimeScope > activationScope();

    private:
        bool newInstanceActivated();

        std::shared_ptr< void > activate();

        std::shared_ptr< IComponentRegistration > componentRegistration_;
        std::shared_ptr< IResolveOperation > context_;
        std::shared_ptr< ISharingLifetimeScope > activationScope_;
        std::shared_ptr< void > newInstance_;
        bool executed_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_INSTANCE_LOOKUP_H_ */