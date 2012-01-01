#ifndef		HYPODERMIC_LIFETIME_SCOPE_H_
# define	HYPODERMIC_LIFETIME_SCOPE_H_

# include <boost/thread.hpp>
# include <boost/unordered_map.hpp>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/Action.h>
# include <Hypodermic/ContainerBuilder.h>
# include <Hypodermic/ISharingLifetimeScope.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;


	class LifetimeScope : public ISharingLifetimeScope
	{
	public:
        static const boost::uuids::uuid selfRegistrationId;

        
        LifetimeScope(IComponentRegistry* componentRegistry);

		ISharingLifetimeScope* parentLifetimeScope();

		ISharingLifetimeScope* rootLifetimeScope();

        IComponentRegistry* componentRegistry();

		void* resolveComponent(IComponentRegistration* registration);

		void* getOrCreateAndShare(const boost::uuids::uuid& id, Func< void, void* > creator);

	protected:
		LifetimeScope(IComponentRegistry* componentRegistry, LifetimeScope* parent);

	private:
		void initialize();

		void initialize(IComponentRegistry* componentRegistry);


		boost::recursive_mutex mutex_;
		boost::unordered_map< boost::uuids::uuid, void* > sharedInstances_;
		IComponentRegistry* componentRegistry_;
		ISharingLifetimeScope* root_;
		ISharingLifetimeScope* parent_;

		static Action< ContainerBuilder* > noConfiguration_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_LIFETIME_SCOPE_H_ */