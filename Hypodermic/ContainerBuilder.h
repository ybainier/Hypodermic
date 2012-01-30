#ifndef		HYPODERMIC_CONTAINER_BUILDER_H_
# define	HYPODERMIC_CONTAINER_BUILDER_H_

# include <functional>
# include <memory>
# include <stdexcept>
# include <vector>

# include <Hypodermic/IContainer.h>
# include <Hypodermic/SingleRegistrationStyle.h>


namespace Hypodermic
{
	class IComponentContext;
	class IComponentRegistry;
	template <class T, class RegistrationStyleT> class IRegistrationBuilder;


	class ContainerBuilder
	{
        typedef std::function< void(std::shared_ptr< IComponentRegistry >) > ConfigurationCallback;

	public:
		ContainerBuilder();

		template <class T>
		std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > autowireType();

        template <class T>
        std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > registerType(std::function< T*(IComponentContext&) > delegate);

		template <class T>
		std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > registerType();

		template <class T>
		std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > registerInstance(std::shared_ptr< T > instance);

		void registerCallback(ConfigurationCallback configurationCallback);

		std::shared_ptr< IContainer > build();

		void build(std::shared_ptr< IComponentRegistry > componentRegistry);


	private:
		std::vector< ConfigurationCallback > configurationCallbacks_;
		bool wasBuilt_;
	};

} // namespace Hypodermic


# include <Hypodermic/ContainerBuilder.hpp>

#endif /* !HYPODERMIC_CONTAINER_BUILDER_H_ */