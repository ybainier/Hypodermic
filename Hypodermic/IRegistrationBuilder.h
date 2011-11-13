#ifndef		HYPODERMIC_IREGISTRATION_BUILDER_H_
# define	HYPODERMIC_IREGISTRATION_BUILDER_H_

# include <vector>


namespace Hypodermic
{
	class IInstanceActivator;
	template <class T> class RegistrationBuilder;
	class RegistrationData;


	template <class T>
	class IRegistrationBuilder
	{
	public:
		virtual RegistrationData& registrationData() = 0;
		virtual IInstanceActivator* activator() = 0;
		virtual IRegistrationBuilder< T >* singleInstance() = 0;

		template <class ServiceT>
		IRegistrationBuilder< T >* as()
		{
			return static_cast< RegistrationBuilder< T >* >(this)->as< ServiceT >();
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IREGISTRATION_BUILDER_H_ */