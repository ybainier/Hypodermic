#ifndef     HYPODERMIC_INSTANCE_LOOKUP_H_
# define    HYPODERMIC_INSTANCE_LOOKUP_H_

# include <stdexcept>
# include <boost/assert.hpp>
# include <boost/bind.hpp>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IComponentLifetime.h>
# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/IInstanceLookup.h>
# include <Hypodermic/IResolveOperation.h>
# include <Hypodermic/ISharingLifetimeScope.h>


namespace Hypodermic
{

    class InstanceLookup : public IComponentContext, public IInstanceLookup
    {
    public:
        InstanceLookup(IComponentRegistration* registration, IResolveOperation* context,
                       ISharingLifetimeScope* mostNestedVisibleScope)
            : componentRegistration_(registration)
            , context_(context)
            , activationScope_(nullptr)
            , newInstance_(nullptr)
            , executed_(false)
        {
            BOOST_ASSERT(registration != nullptr);
            BOOST_ASSERT(context != nullptr);

            if (mostNestedVisibleScope == nullptr)
                throw std::invalid_argument("mostNestedVisibleScope");
            activationScope_ = componentRegistration_->lifetime()->findScope(mostNestedVisibleScope);
        }

        void* execute()
        {
            if (executed_)
                throw std::logic_error("Activation already executed");

            executed_ = true;

            void* instance = nullptr;
            if (componentRegistration_->sharing() == InstanceSharing::None)
                instance = activate();
            else
            {
                instance = activationScope_->getOrCreateAndShare(
                                                boost::uuids::uuid(),
                                                Func< void, void* >(boost::bind(&InstanceLookup::activate, this))
                                             );
            }
            //InstanceLookupEnding(this, new InstanceLookupEndingEventArgs(this, NewInstanceActivated));

            return instance;
        }

        void complete()
        {
            if (newInstanceActivated())
            {
                //        CompletionBeginning(this, new InstanceLookupCompletionBeginningEventArgs(this));

                //        _componentRegistration.RaiseActivated(this, Parameters, _newInstance);

                //        CompletionEnding(this, new InstanceLookupCompletionEndingEventArgs(this));
            }
        }

        IComponentRegistry* componentRegistry()
        {
            return activationScope_->componentRegistry();
        }

        void* resolveComponent(IComponentRegistration* registration)
        {
            return context_->getOrCreateInstance(activationScope_, registration);
        }

        IComponentRegistration* componentRegistration()
        {
            return componentRegistration_;
        }

        ILifetimeScope* activationScope()
        {
            return activationScope_;
        }

    private:
        bool newInstanceActivated()
        {
            return newInstance_ != nullptr;
        }

        void* activate()
        {
        //    _componentRegistration.RaisePreparing(this, ref parameters);

            newInstance_ = componentRegistration_->activator()->activateInstance(this);

        //    if (_componentRegistration.Ownership == InstanceOwnership.OwnedByLifetimeScope)
        //    {
        //        var instanceAsDisposable = _newInstance as IDisposable;
        //        if (instanceAsDisposable != null)
        //            _activationScope.Disposer.AddInstanceForDisposal(instanceAsDisposable);
        //    }

        //    _componentRegistration.RaiseActivating(this, parameters, ref _newInstance);

            return newInstance_;
        }

        IComponentRegistration* componentRegistration_;
        IResolveOperation* context_;
        ISharingLifetimeScope* activationScope_;
        void* newInstance_;
        bool executed_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_INSTANCE_LOOKUP_H_ */