#include <Hypodermic/RegistrationBuilder.h>


namespace Hypodermic
{

	template <class TImplementor>
	IRegistrationBuilder* ContainerBuilder::registerType()
	{
		auto registrationBuilder = RegistrationBuilder::forType< TImplementor >();

		registerCallback([]() -> void
			{
				RegistrationBuilder::registerSingleComponent(cr, registrationBuilder);
			});

		return registrationBuilder;
	}

	void ContainerBuilder::registerCallback(Action< IComponentRegistry* > configurationCallback)
	{
		configurationCallbacks_.push_back(configurationCallback);
	}

} // namespace Hypodermic