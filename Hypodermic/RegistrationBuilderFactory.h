#ifndef     HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# define    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_

# include <typeindex>
# include <vector>

# include <boost/shared_ptr.hpp>
# include <boost/unordered_map.hpp>
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
		static boost::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forDelegate(Func< IComponentContext*, T > delegate);

		template <class T>
		static boost::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forType();

		template <class T, class RegistrationStyleT>
		static void registerSingleComponent(IComponentRegistry* cr, boost::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb);

		template <class T, class RegistrationStyleT>
		static IComponentRegistration* createRegistration(boost::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb);

		static IComponentRegistration* createRegistration(const boost::uuids::uuid& id,
                                                          RegistrationData& registrationData,
                                                          IInstanceActivator* activator,
			                                              std::vector< Service* >& services,
                                                          IComponentRegistration* target,
                                                          const boost::unordered_map< std::type_index, ITypeCaster* >& typeCasters);
	};

} // namespace Hypodermic


# include <Hypodermic/RegistrationBuilderFactory.hpp>

#endif /* !HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_ */