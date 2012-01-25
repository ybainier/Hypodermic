#ifndef		HYPODERMIC_REGISTRATION_BUILDER_H_
# define	HYPODERMIC_REGISTRATION_BUILDER_H_

# include <memory>

# include <Hypodermic/ComponentRegistration.h>
# include <Hypodermic/CurrentLifetimeScope.h>
# include <Hypodermic/DelegateActivator.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/IRegistrationBuilder.h>
# include <Hypodermic/RegistrationData.h>
# include <Hypodermic/RootScopeLifetime.h>
# include <Hypodermic/SingleRegistrationStyle.h>
# include <Hypodermic/TypedService.h>
# include <Hypodermic/TypeIndexWorkaround.h>


namespace Hypodermic
{
	template <class T, class RegistrationStyleT>
	class RegistrationBuilder
        : public std::enable_shared_from_this< RegistrationBuilder< T, RegistrationStyleT > >
        , public IRegistrationBuilder< T, RegistrationStyleT >
	{
	public:
		RegistrationBuilder(std::shared_ptr< Service > defaultService, std::shared_ptr< IInstanceActivator > activator,
                            const RegistrationStyleT& registrationStyle);

		RegistrationData& registrationData();

		std::shared_ptr< IInstanceActivator > activator();

        RegistrationStyleT& registrationStyle();

        const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >& typeCasters() const;

		std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > singleInstance();

        std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > instancePerLifetimeScope();

        std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > instancePerDependency();

		template <class ServiceT>
		std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > as();

	private:
		RegistrationData registrationData_;
		std::shared_ptr< IInstanceActivator > activator_;
        RegistrationStyleT registrationStyle_;
        std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > > typeCasters_;
	};

} // namespace Hypodermic


# include <Hypodermic/RegistrationBuilder.hpp>

#endif /* !HYPODERMIC_REGISTRATION_BUILDER_H_ */