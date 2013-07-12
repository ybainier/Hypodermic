#ifndef		HYPODERMIC_LIFETIME_SCOPE_H_
# define	HYPODERMIC_LIFETIME_SCOPE_H_

# include <functional>
# include <memory>
# include <unordered_map>

# include <boost/thread.hpp>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/BoostUuidHashFunctor.h>
# include <Hypodermic/ContainerBuilder.h>
# include <Hypodermic/ISharingLifetimeScope.h>
# include <Hypodermic/ScopeRestrictedRegistry.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;


	class LifetimeScope : public std::enable_shared_from_this< LifetimeScope >, public ISharingLifetimeScope
	{
	public:
        LifetimeScope(std::shared_ptr< IComponentRegistry > componentRegistry);

		std::shared_ptr< ISharingLifetimeScope > parentLifetimeScope();
		std::shared_ptr< ISharingLifetimeScope > rootLifetimeScope();

        std::shared_ptr< IComponentRegistry > componentRegistry();

		std::shared_ptr< void > resolveComponent(std::shared_ptr< IComponentRegistration > registration);
		std::shared_ptr< void > getOrCreateAndShare(const boost::uuids::uuid& id, std::function< std::shared_ptr< void >() > creator);

        void initialize();

        std::shared_ptr< ILifetimeScope > createLifetimeScope();

        static const boost::uuids::uuid selfRegistrationId;

	protected:
        LifetimeScope(std::shared_ptr< IComponentRegistry > componentRegistry, std::shared_ptr< LifetimeScope > parent);

    private:
        std::shared_ptr< ScopeRestrictedRegistry > createScopeRestrictedRegistry(std::function< void(ContainerBuilder&) > configurationAction);

        std::shared_ptr< IComponentRegistry > componentRegistry_;
		std::shared_ptr< ISharingLifetimeScope > parent_;
		std::weak_ptr< ISharingLifetimeScope > root_;
		std::unordered_map< boost::uuids::uuid, std::shared_ptr< void > > sharedInstances_;

        boost::recursive_mutex mutex_;

		static std::function< void(ContainerBuilder&) > noConfiguration_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_LIFETIME_SCOPE_H_ */