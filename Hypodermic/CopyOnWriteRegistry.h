#ifndef		HYPODERMIC_COPY_ON_WRITE_REGISTRY_H_
# define	HYPODERMIC_COPY_ON_WRITE_REGISTRY_H_

# include <memory>
# include <functional>
# include <vector>

# include <Hypodermic/IComponentRegistry.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IRegistrationSource;


	class CopyOnWriteRegistry : public IComponentRegistry
	{
	public:
        CopyOnWriteRegistry(std::shared_ptr< IComponentRegistry > readRegistry, std::function< std::shared_ptr< IComponentRegistry >() > createWriteRegistry);

        std::shared_ptr< IComponentRegistry > registry();
        std::shared_ptr< IComponentRegistry > writeRegistry();

		std::shared_ptr< IComponentRegistration > getRegistration(std::shared_ptr< Service > service);

		bool isRegistered(std::shared_ptr< Service > service);

		void addRegistration(std::shared_ptr< IComponentRegistration > registration);

        void addRegistration(std::shared_ptr< IComponentRegistration > registration, bool preserveDefaults);

        void addRegistrationSource(std::shared_ptr< IRegistrationSource > source);

		std::vector< std::shared_ptr< IComponentRegistration > > registrations();

		std::vector< std::shared_ptr< IComponentRegistration > > registrationsFor(std::shared_ptr< Service > service);

        bool hasLocalComponents() const;

    private:
        std::shared_ptr< IComponentRegistry > readRegistry_;
        std::function< std::shared_ptr< IComponentRegistry >() > createWriteRegistry_;
        std::shared_ptr< IComponentRegistry > writeRegistry_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COPY_ON_WRITE_REGISTRY_H_ */