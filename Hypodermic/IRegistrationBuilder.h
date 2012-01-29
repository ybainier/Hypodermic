#ifndef		HYPODERMIC_IREGISTRATION_BUILDER_H_
# define	HYPODERMIC_IREGISTRATION_BUILDER_H_

# include <memory>
# include <string>
# include <type_traits>
# include <unordered_map>
# include <vector>

# include <Hypodermic/ITypeCaster.h>


namespace Hypodermic
{
	class IInstanceActivator;
	template <class T, class RegistrationStyleT> class RegistrationBuilder;
	class RegistrationData;


	template <class T, class RegistrationStyleT>
	class IRegistrationBuilder
	{
	public:
		virtual RegistrationData& registrationData() = 0;
		virtual std::shared_ptr< IInstanceActivator > activator() = 0;
        virtual RegistrationStyleT& registrationStyle() = 0;

        virtual const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >& typeCasters() const = 0;

		virtual std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > singleInstance() = 0;
        virtual std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > instancePerLifetimeScope() = 0;
        virtual std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > instancePerDependency() = 0;

		template <class ServiceT>
		std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > as()
		{
            static_assert(std::is_same< T, ServiceT >::value || std::is_convertible< T, ServiceT >::value || std::is_base_of< ServiceT, T >::value,
                          "IRegistrationBuilder< T, RegistrationStyleT >::as< ServiceT >() requires T to be convertible to ServiceT");

			return static_cast< RegistrationBuilder< T, RegistrationStyleT >* >(this)->as< ServiceT >();
		}

		template <class ServiceT>
		std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > named(const std::string& serviceName)
		{
            static_assert(std::is_same< T, ServiceT >::value || std::is_convertible< T, ServiceT >::value || std::is_base_of< ServiceT, T >::value,
                          "IRegistrationBuilder< T, RegistrationStyleT >::named< ServiceT >() requires T to be convertible to ServiceT");

			return static_cast< RegistrationBuilder< T, RegistrationStyleT >* >(this)->named< ServiceT >(serviceName);
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IREGISTRATION_BUILDER_H_ */