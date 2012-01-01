#ifndef     HYPODERMIC_RESOLVE_OPERATION_H_
# define    HYPODERMIC_RESOLVE_OPERATION_H_

# include <deque>
# include <stdexcept>

# include <Hypodermic/CircularDependencyDetector.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IResolveOperation.h>


namespace Hypodermic
{
    class ISharingLifetimeScope;
    class IComponentRegistration;


    class ResolveOperation : public IComponentContext, public IResolveOperation
    {
    public:
        ResolveOperation(ISharingLifetimeScope* mostNestedLifetimeScope);

        IComponentRegistry* componentRegistry();

        void* resolveComponent(IComponentRegistration* registration);

        void* execute(IComponentRegistration* registration);

        void* getOrCreateInstance(ISharingLifetimeScope* currentOperationScope,
                                  IComponentRegistration* registration);

    private:
        void completeActivations();

        void resetSuccessfulActivations();

        void end(std::exception& exception);

        void end();


        ISharingLifetimeScope* mostNestedLifetimeScope_;
        int callDepth_;
        bool ended_;

        std::deque< InstanceLookup* > activationStack_;
        std::deque< InstanceLookup* > successfulActivations_;
        CircularDependencyDetector circularDependencyDetector_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_RESOLVE_OPERATION_H_ */