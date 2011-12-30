#ifndef		HYPODERMIC_IREGISTRATION_BUILDER_H_
# define	HYPODERMIC_IREGISTRATION_BUILDER_H_

# include <vector>


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

		virtual IRegistrationBuilder< T, RegistrationStyleT >* singleInstance() = 0;
        virtual IRegistrationBuilder< T, RegistrationStyleT >* instancePerLifetimeScope() = 0;
        virtual IRegistrationBuilder< T, RegistrationStyleT >* instancePerDependency() = 0;

		template <class ServiceT>
		IRegistrationBuilder< T, RegistrationStyleT >* as()
		{
			return static_cast< RegistrationBuilder< T, RegistrationStyleT >* >(this)->as< ServiceT >();
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IREGISTRATION_BUILDER_H_ */