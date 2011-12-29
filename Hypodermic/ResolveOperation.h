#ifndef     HYPODERMIC_RESOLVE_OPERATION_H_
# define    HYPODERMIC_RESOLVE_OPERATION_H_

# include <deque>
# include <stdexcept>

# include <boost/foreach.hpp>

# include <Hypodermic/CircularDependencyDetector.h>
# include <Hypodermic/DependencyResolutionException.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/InstanceLookup.h>
# include <Hypodermic/IResolveOperation.h>
# include <Hypodermic/ISharingLifetimeScope.h>


namespace Hypodermic
{

    class ResolveOperation : public IComponentContext, public IResolveOperation
    {
    public:
        ResolveOperation(ISharingLifetimeScope* mostNestedLifetimeScope)
            : mostNestedLifetimeScope_(mostNestedLifetimeScope)
            , callDepth_(0)
            , ended_(false)
        {
            if (mostNestedLifetimeScope == nullptr)
                throw std::invalid_argument("mostNestedLifetimeScope");
            resetSuccessfulActivations();
        }

        IComponentRegistry* componentRegistry()
        {
            return mostNestedLifetimeScope_->componentRegistry();
        }

        void* resolveComponent(IComponentRegistration* registration)
        {
            return getOrCreateInstance(mostNestedLifetimeScope_, registration);
        }

        void* execute(IComponentRegistration* registration)
        {
            void* result = nullptr;

            try
            {
                result = resolveComponent(registration);
            }
            catch (DependencyResolutionException& ex)
            {
                end(ex);
                throw;
            }
            catch (std::exception& ex)
            {
                end(ex);
                throw DependencyResolutionException(ex.what());
            }

            end();
            return result;
        }

        void* getOrCreateInstance(ISharingLifetimeScope* currentOperationScope,
                                  IComponentRegistration* registration)
        {
            if (currentOperationScope == nullptr)
                throw std::invalid_argument("currentOperationScope");
            if (registration == nullptr)
                throw std::invalid_argument("registration");
            //if (ended_)
            //    throw ObjectDisposedException(ResolveOperationResources.TemporaryContextDisposed, innerException: null);

            circularDependencyDetector_.checkForCircularDependency(registration, activationStack_, ++callDepth_);

            auto activation = new InstanceLookup(registration, this, currentOperationScope);

            activationStack_.push_back(activation);

            //InstanceLookupBeginning(this, new InstanceLookupBeginningEventArgs(activation));

            auto instance = activation->execute();
            successfulActivations_.push_back(activation);

            activationStack_.pop_back();

            //if (activationStack_.size() == 0)
            //    CompleteActivations();

            --callDepth_;

            return instance;
        }

    private:
        void completeActivations()
        {
            auto completedActivations = successfulActivations_;
            resetSuccessfulActivations();

            BOOST_FOREACH(auto activation, completedActivations)
                activation->complete();
        }

        void resetSuccessfulActivations()
        {
            successfulActivations_.clear();
        }

        void end(std::exception& exception)
        {
            if (!ended_)
            {
                ended_ = true;
                //CurrentOperationEnding(this, new ResolveOperationEndingEventArgs(this, exception));
            }
        }

        void end()
        {
            if (!ended_)
            {
                ended_ = true;
                //CurrentOperationEnding(this, new ResolveOperationEndingEventArgs(this));
            }
        }

        ISharingLifetimeScope* mostNestedLifetimeScope_;
        int callDepth_;
        bool ended_;

        std::deque< InstanceLookup* > activationStack_;
        std::deque< InstanceLookup* > successfulActivations_;
        CircularDependencyDetector circularDependencyDetector_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_RESOLVE_OPERATION_H_ */