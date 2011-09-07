#ifndef		IREGISTRATION_BUILDER_H_
# define	IREGISTRATION_BUILDER_H_

# include <vector>

namespace Hypodermic
{
	class IInstanceActivator;
	class RegistrationData;

	template <class T>
	class IRegistrationBuilder
	{
	public:
		virtual RegistrationData& registrationData() = 0;
		virtual IInstanceActivator* activator() = 0;
	};

} // namespace Hypodermic



#endif /* !IREGISTRATION_BUILDER_H_ */