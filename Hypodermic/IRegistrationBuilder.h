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
	class RegistrationData;


	template
    <
        class T,
        class RegistrationStyleT,
        template <class /*T*/, class /*RegistrationStyleT*/> class RegistrationBuilderT
    >
	class IRegistrationBuilder
	{
	public:
        typedef IRegistrationBuilder< T, RegistrationStyleT, RegistrationBuilderT > SelfType;
        typedef RegistrationBuilderT< T, RegistrationStyleT > RegistrationBuilderImplementationType;

        virtual ~IRegistrationBuilder() = 0;

		virtual RegistrationData& registrationData() = 0;
		virtual std::shared_ptr< IInstanceActivator > activator() = 0;
        virtual RegistrationStyleT& registrationStyle() = 0;

        virtual const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >& typeCasters() const = 0;

		virtual std::shared_ptr< SelfType > singleInstance() = 0;
        virtual std::shared_ptr< SelfType > instancePerLifetimeScope() = 0;
        virtual std::shared_ptr< SelfType > instancePerDependency() = 0;

		template <class ServiceT>
		std::shared_ptr< SelfType > as()
		{
            static_assert(std::is_same< T, ServiceT >::value || std::is_base_of< ServiceT, T >::value,
                          "IRegistrationBuilder< T, RegistrationStyleT, RegistrationBuilderT >::as< ServiceT >() requires ServiceT to be base of T");

			return static_cast< RegistrationBuilderImplementationType* >(this)->as< ServiceT >();
		}

        virtual std::shared_ptr< SelfType > asSelf() = 0;

		template <class ServiceT>
		std::shared_ptr< SelfType > named(const std::string& serviceName)
		{
            static_assert(std::is_same< T, ServiceT >::value || std::is_base_of< ServiceT, T >::value,
                          "IRegistrationBuilder< T, RegistrationStyleT, RegistrationBuilderT >::as< ServiceT >() requires ServiceT to be base of T");

			return static_cast< RegistrationBuilderImplementationType* >(this)->named< ServiceT >(serviceName);
		}
	};

	template
    <
        class T,
        class RegistrationStyleT,
        template <class /*T*/, class /*RegistrationStyleT*/> class RegistrationBuilderT
    >
    inline IRegistrationBuilder< T, RegistrationStyleT, RegistrationBuilderT >::~IRegistrationBuilder() {}

} // namespace Hypodermic


#endif /* !HYPODERMIC_IREGISTRATION_BUILDER_H_ */