#ifndef     HYPODERMIC_RESOLVE_OPERATION_H_
# define    HYPODERMIC_RESOLVE_OPERATION_H_

# include <deque>
# include <memory>
# include <stdexcept>

# include <Hypodermic/CircularDependencyDetector.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IResolveOperation.h>


namespace Hypodermic
{
    class ISharingLifetimeScope;
    class IComponentRegistration;


    class ResolveOperation : public std::enable_shared_from_this< ResolveOperation >, public IComponentContext, public IResolveOperation
    {
    public:
        ResolveOperation(std::shared_ptr< ISharingLifetimeScope > mostNestedLifetimeScope);

        std::shared_ptr< IComponentRegistry > componentRegistry();

        std::shared_ptr< void > resolveComponent(std::shared_ptr< IComponentRegistration > registration);

        std::shared_ptr< void > execute(std::shared_ptr< IComponentRegistration > registration);

        std::shared_ptr< void > getOrCreateInstance(std::shared_ptr< ISharingLifetimeScope > currentOperationScope,
                                                    std::shared_ptr< IComponentRegistration > registration);

    private:
        void completeActivations();

        void resetSuccessfulActivations();

        void end(std::exception& exception);

        void end();


        std::shared_ptr< ISharingLifetimeScope > mostNestedLifetimeScope_;
        int callDepth_;
        bool ended_;

        std::deque< std::shared_ptr< InstanceLookup > > activationStack_;
        std::deque< std::shared_ptr< InstanceLookup > > successfulActivations_;
        CircularDependencyDetector circularDependencyDetector_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_RESOLVE_OPERATION_H_ */