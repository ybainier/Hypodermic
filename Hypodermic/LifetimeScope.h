#ifndef		HYPODERMIC_LIFETIME_SCOPE_H_
# define	HYPODERMIC_LIFETIME_SCOPE_H_

# include <stdexcept>

# include <boost/assert.hpp>
# include <boost/thread.hpp>
# include <boost/unordered_map.hpp>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/Action.h>
# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/ISharingLifetimeScope.h>
# include <Hypodermic/ResolveOperation.h>


namespace Hypodermic
{
    class ContainerBuilder;

	class LifetimeScope : public ISharingLifetimeScope
	{
	public:
		LifetimeScope(IComponentRegistry* componentRegistry)
		{
			initialize(componentRegistry);
		}

		ISharingLifetimeScope* parentLifetimeScope()
		{
			return parent_;
		}

		ISharingLifetimeScope* rootLifetimeScope()
		{
			return root_;
		}

        IComponentRegistry* componentRegistry()
        {
            return componentRegistry_;
        }

        static const boost::uuids::uuid& selfRegistrationId()
        {
            return selfRegistrationId_;
        }

		void* resolveComponent(IComponentRegistration* registration)
		{
			if (registration == nullptr)
				throw std::invalid_argument("registration");

			//CheckNotDisposed();

			{
				boost::lock_guard< decltype(mutex_) > lock(mutex_);

                ResolveOperation operation(this);
                //ResolveOperationBeginning(this, new ResolveOperationBeginningEventArgs(operation));
                return operation.execute(registration);
			}
		}

		void* getOrCreateAndShare(const boost::uuids::uuid& id, Func< void, void* > creator)
		{
			boost::lock_guard< decltype(mutex_) > lock(mutex_);

			void* result = nullptr;
			if (sharedInstances_.count(id) == 0)
			{
				result = creator();
				sharedInstances_[id] = result;
			}
			else
				result = sharedInstances_[id];

			return result;
		}

	protected:
		LifetimeScope(IComponentRegistry* componentRegistry, LifetimeScope* parent)
		{
			initialize(componentRegistry);

			BOOST_ASSERT(parent != nullptr);
			parent_ = parent;
			root_ = parent_->rootLifetimeScope();
		}

	private:
		void initialize()
		{
			sharedInstances_[selfRegistrationId_] = this;
		}

		void initialize(IComponentRegistry* componentRegistry)
		{
			initialize();

			BOOST_ASSERT(componentRegistry != nullptr);
			componentRegistry_ = componentRegistry;
			root_ = this;
		}


		boost::recursive_mutex mutex_;
		boost::unordered_map< boost::uuids::uuid, void* > sharedInstances_;
		IComponentRegistry* componentRegistry_;
		ISharingLifetimeScope* root_;
		ISharingLifetimeScope* parent_;

		static boost::uuids::uuid selfRegistrationId_;
		static Action< ContainerBuilder* > noConfiguration_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_LIFETIME_SCOPE_H_ */