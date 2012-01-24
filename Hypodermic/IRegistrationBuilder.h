#ifndef		HYPODERMIC_IREGISTRATION_BUILDER_H_
# define	HYPODERMIC_IREGISTRATION_BUILDER_H_

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
		virtual IInstanceActivator* activator() = 0;
        virtual RegistrationStyleT& registrationStyle() = 0;

        virtual const std::unordered_map< std::type_index, ITypeCaster* >& typeCasters() const = 0;

		virtual IRegistrationBuilder< T, RegistrationStyleT >* singleInstance() = 0;
        virtual IRegistrationBuilder< T, RegistrationStyleT >* instancePerLifetimeScope() = 0;
        virtual IRegistrationBuilder< T, RegistrationStyleT >* instancePerDependency() = 0;

		template <class ServiceT>
		IRegistrationBuilder< T, RegistrationStyleT >* as()
		{
            static_assert(std::is_same< T, ServiceT >::value || std::is_convertible< T, ServiceT >::value || std::is_base_of< ServiceT, T >::value,
                          "IRegistrationBuilder< T, RegistrationStyleT >::as< ServiceT >() requires T to be convertible to ServiceT");

			return static_cast< RegistrationBuilder< T, RegistrationStyleT >* >(this)->as< ServiceT >();
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IREGISTRATION_BUILDER_H_ */