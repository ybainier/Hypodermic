#ifndef		HYPODERMIC_LIFETIME_SCOPE_H_
# define	HYPODERMIC_LIFETIME_SCOPE_H_

# include <functional>
# include <unordered_map>

# include <boost/thread.hpp>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/BoostUuidHashFunctor.h>
# include <Hypodermic/ContainerBuilder.h>
# include <Hypodermic/ISharingLifetimeScope.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;


	class LifetimeScope : public ISharingLifetimeScope
	{
	public:
        LifetimeScope(IComponentRegistry* componentRegistry);

		ISharingLifetimeScope* parentLifetimeScope();
		ISharingLifetimeScope* rootLifetimeScope();

        IComponentRegistry* componentRegistry();

		void* resolveComponent(IComponentRegistration* registration);
		void* getOrCreateAndShare(const boost::uuids::uuid& id, std::function< void*() > creator);

        static const boost::uuids::uuid selfRegistrationId;

	protected:
		LifetimeScope(IComponentRegistry* componentRegistry, LifetimeScope* parent);

	private:
		IComponentRegistry* componentRegistry_;
		ISharingLifetimeScope* parent_;
		ISharingLifetimeScope* root_;
		std::unordered_map< boost::uuids::uuid, void* > sharedInstances_;

        boost::recursive_mutex mutex_;

		static std::function< void(ContainerBuilder*) > noConfiguration_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_LIFETIME_SCOPE_H_ */