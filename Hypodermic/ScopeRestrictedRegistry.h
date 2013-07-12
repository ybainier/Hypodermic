#ifndef		HYPODERMIC_SCOPE_RESTRICTED_REGISTRY_H_
# define	HYPODERMIC_SCOPE_RESTRICTED_REGISTRY_H_

# include <memory>

# include <Hypodermic/ComponentRegistry.h>
# include <Hypodermic/IComponentLifetime.h>


namespace Hypodermic
{

	class ScopeRestrictedRegistry : public ComponentRegistry
	{
	public:
        ScopeRestrictedRegistry();

        virtual void addRegistration(std::shared_ptr< IComponentRegistration > registration, bool preserveDefaults);

    private:
        std::shared_ptr< IComponentLifetime > restrictedRootScopeLifetime_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_SCOPE_RESTRICTED_REGISTRY_H_ */