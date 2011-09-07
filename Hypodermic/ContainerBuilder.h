#ifndef		CONTAINER_BUILDER_H_
# define	CONTAINER_BUILDER_H_

# include <stdexcept>
# include <vector>

# include <boost/foreach.hpp>

# include <Hypodermic/Action.h>
# include <Hypodermic/Container.h>
# include <Hypodermic/Func.h>
# include <Hypodermic/RegistrationBuilder.h>


namespace Hypodermic
{
	class IComponentContext;
	class IComponentRegistry;
	class IContainer;
	template <class T> class IRegistrationBuilder;


	class ContainerBuilder
	{
		typedef Action< IComponentRegistry* > ConfigurationCallback;

	public:
		ContainerBuilder()
		{
			wasBuilt_ = false;
		}

		template <class T>
		IRegistrationBuilder< T >* registerConstruction(Func< IComponentContext*, T > delegate)
		{
			auto rb = RegistrationBuilderFactory::forDelegate(delegate);
			
			registerCallback(ConfigurationCallback(
				[rb](IComponentRegistry* cr) -> void
				{
					return RegistrationBuilderFactory::registerSingleComponent(cr, rb);
				}));

			return rb;
		}

		template <class T>
		IRegistrationBuilder< T >* registerType()
		{
			auto rb = RegistrationBuilderFactory::forType< T >();

			registerCallback(ConfigurationCallback(
				[rb](IComponentRegistry* cr) -> void
			{
				return RegistrationBuilderFactory::registerSingleComponent(cr, rb);
			}));

			return rb;
		}

		void registerCallback(ConfigurationCallback configurationCallback)
		{
			configurationCallbacks_.push_back(configurationCallback);
		}


		Container* build()
		{
			auto result = new Container;
			build(result->componentRegistry());

			return result;
		}


		void build(IComponentRegistry* componentRegistry)
		{
			if (componentRegistry == nullptr)
				throw std::invalid_argument("componentRegistry");

			if (wasBuilt_)
				throw std::logic_error("ContainerBuilder::build() can only be called once");

			wasBuilt_ = true;

			BOOST_FOREACH(auto callback, configurationCallbacks_)
				callback(componentRegistry);
		}


	private:
		std::vector< ConfigurationCallback > configurationCallbacks_;
		bool wasBuilt_;
	};

} // namespace Hypodermic



#endif /* !CONTAINER_BUILDER_H_ */