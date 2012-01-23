#ifndef     HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# define    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_

# include <functional>
# include <memory>
# include <typeindex>
# include <unordered_map>
# include <vector>

# include <boost/uuid/uuid.hpp>

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
		static std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forDelegate(std::function< T*(IComponentContext*) > delegate);

		template <class T>
		static std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forType();

        template <class T>
		static std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forInstance(T* instance);

		template <class T, class RegistrationStyleT>
		static void registerSingleComponent(IComponentRegistry* cr, std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb);

		template <class T, class RegistrationStyleT>
		static IComponentRegistration* createRegistration(std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb);

		static IComponentRegistration* createRegistration(const boost::uuids::uuid& id,
                                                          RegistrationData& registrationData,
                                                          IInstanceActivator* activator,
			                                              std::vector< Service* >& services,
                                                          IComponentRegistration* target,
                                                          const std::unordered_map< std::type_index, ITypeCaster* >& typeCasters);
	};

} // namespace Hypodermic


# include <Hypodermic/RegistrationBuilderFactory.hpp>

#endif /* !HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_ */