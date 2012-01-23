#ifndef		HYPODERMIC_REGISTRATION_BUILDER_H_
# define	HYPODERMIC_REGISTRATION_BUILDER_H_

# include <typeindex>

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


namespace Hypodermic
{
	template <class T, class RegistrationStyleT>
	class RegistrationBuilder : public IRegistrationBuilder< T, RegistrationStyleT >
	{
	public:
		RegistrationBuilder(Service* defaultService, IInstanceActivator* activator,
                            const RegistrationStyleT& registrationStyle);

		RegistrationData& registrationData();

		IInstanceActivator* activator();

        RegistrationStyleT& registrationStyle();

        const std::unordered_map< std::type_index, ITypeCaster* >& typeCasters() const;

		IRegistrationBuilder< T, RegistrationStyleT >* singleInstance();

        IRegistrationBuilder< T, RegistrationStyleT >* instancePerLifetimeScope();

        IRegistrationBuilder< T, RegistrationStyleT >* instancePerDependency();

		template <class ServiceT>
		IRegistrationBuilder< T, RegistrationStyleT >* as();

	private:
		RegistrationData registrationData_;
		IInstanceActivator* activator_;
        RegistrationStyleT registrationStyle_;
        std::unordered_map< std::type_index, ITypeCaster* > typeCasters_;
	};

} // namespace Hypodermic


# include <Hypodermic/RegistrationBuilder.hpp>

#endif /* !HYPODERMIC_REGISTRATION_BUILDER_H_ */