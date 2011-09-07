namespace Hypodermic
{

	template <class TImplementor>
	IRegistrationBuilder* RegistrationBuilder::forType()
	{
		return new RegistrationBuilder< TImplementor >(new TypedService(typeof(TImplementor)));
	}

} // namespace Hypodermic