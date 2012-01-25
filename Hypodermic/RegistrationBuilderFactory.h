#ifndef     HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# define    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_

# include <functional>
# include <memory>
# include <unordered_map>
# include <vector>

# include <boost/uuid/uuid.hpp>

# include <Hypodermic/SingleRegistrationStyle.h>
# include <Hypodermic/TypeIndexWorkaround.h>


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
		static std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forDelegate(std::function< T*(IComponentContext&) > delegate);

		template <class T>
		static std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forType();

        template <class T>
		static std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > forInstance(std::shared_ptr< T > instance);

		template <class T, class RegistrationStyleT>
		static void registerSingleComponent(std::shared_ptr< IComponentRegistry > cr, std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb);

		template <class T, class RegistrationStyleT>
		static std::shared_ptr< IComponentRegistration > createRegistration(std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb);

		static std::shared_ptr< IComponentRegistration > createRegistration(const boost::uuids::uuid& id,
                                                                            RegistrationData& registrationData,
                                                                            std::shared_ptr< IInstanceActivator > activator,
			                                                                std::vector< std::shared_ptr< Service > > services,
                                                                            std::shared_ptr< IComponentRegistration > target,
                                                                            const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >& typeCasters);
	};

} // namespace Hypodermic


# include <Hypodermic/RegistrationBuilderFactory.hpp>

#endif /* !HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_ */