#ifndef		HYPODERMIC_CONTAINER_BUILDER_H_
# define	HYPODERMIC_CONTAINER_BUILDER_H_

# include <functional>
# include <memory>
# include <stdexcept>
# include <vector>

# include <Hypodermic/IContainer.h>
# include <Hypodermic/IRegistrationSource.h>
# include <Hypodermic/RegistrationBuilder.h>
# include <Hypodermic/SingleRegistrationStyle.h>


namespace Hypodermic
{
	class IComponentContext;
	class IComponentRegistry;


	class ContainerBuilder
	{
        typedef std::function< void(const std::shared_ptr< IComponentRegistry >&) > ConfigurationCallback;

	public:
        template <class T>
        struct RegistrationBuilderInterface
        {
            typedef SingleRegistrationStyle RegistrationStyleType;
            typedef IRegistrationBuilder< T, RegistrationStyleType, RegistrationBuilder > Type;
            typedef RegistrationBuilder< T, RegistrationStyleType > ImplementationType;
        };

		ContainerBuilder();

		template <class T>
		typename RegistrationBuilderInterface< T >::Type& autowireType();

        template <class T>
        typename RegistrationBuilderInterface< T >::Type& registerType(const std::function< std::shared_ptr< T >(IComponentContext&) >& delegate);

		template <class T>
		typename RegistrationBuilderInterface< T >::Type& registerType();

		template <class T>
        typename RegistrationBuilderInterface< T >::Type& registerInstance(const std::shared_ptr< T >& instance);

        void registerSource(const std::shared_ptr< IRegistrationSource >& registrationSource);

		void registerCallback(const ConfigurationCallback& configurationCallback);

        void addRegistrations(const ContainerBuilder& containerBuilder);

		std::shared_ptr< IContainer > build();

		void build(const std::shared_ptr< IComponentRegistry >& componentRegistry);


	private:
		std::vector< ConfigurationCallback > configurationCallbacks_;
		bool wasBuilt_;
	};

} // namespace Hypodermic


# include <Hypodermic/ContainerBuilder.hpp>

#endif /* !HYPODERMIC_CONTAINER_BUILDER_H_ */