#ifndef     HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# define    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_

# include <vector>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/Func.h>
# include <Hypodermic/SingleRegistrationStyle.h>


namespace Hypodermic
{
    class IComponentContext;
    class IComponentRegistration;
    class IComponentRegistry;
    class IInstanceActivator;
    template <class T, class RegistrationStyleT> class IRegistrationBuilder;
    class RegistrationData;
    class Service;


    class RegistrationBuilderFactory
	{
	public:
		template <class T>
		static IRegistrationBuilder< T, SingleRegistrationStyle >* forDelegate(Func< IComponentContext*, T > delegate);

		template <class T>
		static IRegistrationBuilder< T, SingleRegistrationStyle >* forType();

		template <class T, class RegistrationStyleT>
		static void registerSingleComponent(IComponentRegistry* cr, IRegistrationBuilder< T, RegistrationStyleT >* rb);

		template <class T, class RegistrationStyleT>
		static IComponentRegistration* createRegistration(IRegistrationBuilder< T, RegistrationStyleT >* rb);

		static IComponentRegistration* createRegistration(const boost::uuids::uuid& id,
                                                          RegistrationData& registrationData,
                                                          IInstanceActivator* activator,
			                                              std::vector< Service* >& services,
                                                          IComponentRegistration* target,
                                                          const std::vector< ITypeCaster* >& typeCasters);
	};

} // namespace Hypodermic


# include <Hypodermic/RegistrationBuilderFactory.hpp>

#endif /* !HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_ */