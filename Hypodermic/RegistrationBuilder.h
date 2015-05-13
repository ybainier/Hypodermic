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
        , public IRegistrationBuilder< T, RegistrationStyleT, RegistrationBuilder >
	{
	public:
        typedef IRegistrationBuilder< T, RegistrationStyleT, Hypodermic::RegistrationBuilder > ParentType;
        typedef typename ParentType::RegistrationBuilderImplementationType SelfType;

		RegistrationBuilder(std::shared_ptr< Service > defaultService, std::shared_ptr< IInstanceActivator > activator,
                            const RegistrationStyleT& registrationStyle);

		RegistrationData& registrationData();

		std::shared_ptr< IInstanceActivator > activator();

        RegistrationStyleT& registrationStyle();

        const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >& typeCasters() const;

		ParentType& singleInstance();

        ParentType& instancePerLifetimeScope();

        ParentType& instancePerDependency();

		template <class ServiceT>
        ParentType& as();

        ParentType& as(std::shared_ptr< Service > service, std::shared_ptr< ITypeCaster > typeCaster);

        ParentType& asSelf();

		template <class ServiceT>
        ParentType& named(const std::string& serviceName);

        ParentType& targeting(std::shared_ptr< IComponentRegistration > target);

        ParentType& onPreparing(std::function< void(IPreparingData&) > callback);

        ParentType& onActivating(std::function< void(IActivatingData< T >&) > callback);

        ParentType& onActivated(std::function< void(IActivatedData< T >&) > callback);

	private:
        template <class ServiceT>
        ParentType& named(const std::string& serviceName, const std::type_info& serviceTypeInfo);

		RegistrationData registrationData_;
		std::shared_ptr< IInstanceActivator > activator_;
        RegistrationStyleT registrationStyle_;
        std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > > typeCasters_;
	};

} // namespace Hypodermic


# include <Hypodermic/RegistrationBuilder.hpp>

#endif /* !HYPODERMIC_REGISTRATION_BUILDER_H_ */