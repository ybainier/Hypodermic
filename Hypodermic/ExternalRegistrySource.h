#ifndef		HYPODERMIC_EXTERNAL_REGISTRY_SOURCE_H_
# define	HYPODERMIC_EXTERNAL_REGISTRY_SOURCE_H_

# include <memory>

# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/IRegistrationSource.h>


namespace Hypodermic
{

	class ExternalRegistrySource : public IRegistrationSource
	{
	public:
        ExternalRegistrySource(std::shared_ptr< IComponentRegistry > registry);

        std::vector< std::shared_ptr< IComponentRegistration > >
        registrationsFor(std::shared_ptr< Service > service,
                         std::function< std::vector< std::shared_ptr< IComponentRegistration > >(std::shared_ptr< Service > service) > registrationAccessor);

    private:
        std::shared_ptr< IComponentRegistry > registry_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_EXTERNAL_REGISTRY_SOURCE_H_ */