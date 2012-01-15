#ifndef		HYPODERMIC_CONTAINER_BUILDER_H_
# define	HYPODERMIC_CONTAINER_BUILDER_H_

# include <stdexcept>
# include <vector>

# include <boost/foreach.hpp>

# include <Hypodermic/Action.h>
# include <Hypodermic/Func.h>
# include <Hypodermic/SingleRegistrationStyle.h>


namespace Hypodermic
{
    class Container;
	class IComponentContext;
	class IComponentRegistry;
	class IContainer;
	template <class T, class RegistrationStyleT> class IRegistrationBuilder;


	class ContainerBuilder
	{
		typedef Action< IComponentRegistry* > ConfigurationCallback;

	public:
		ContainerBuilder();

		template <class T>
		IRegistrationBuilder< T, SingleRegistrationStyle >* registerType(Func< IComponentContext*, T > delegate);

		template <class T>
		IRegistrationBuilder< T, SingleRegistrationStyle >* registerType();

		template <class T>
		IRegistrationBuilder< T, SingleRegistrationStyle >* registerType(T instance);

		void registerCallback(ConfigurationCallback configurationCallback);

		IContainer* build();

		void build(IComponentRegistry* componentRegistry);


	private:
		std::vector< ConfigurationCallback > configurationCallbacks_;
		bool wasBuilt_;
	};

} // namespace Hypodermic


# include <Hypodermic/ContainerBuilder.hpp>

#endif /* !HYPODERMIC_CONTAINER_BUILDER_H_ */