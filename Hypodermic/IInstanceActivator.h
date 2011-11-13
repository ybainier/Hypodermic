#ifndef		HYPODERMIC_IINSTANCE_ACTIVATOR_H_
# define	HYPODERMIC_IINSTANCE_ACTIVATOR_H_


namespace Hypodermic
{
	class IComponentContext;


	class IInstanceActivator
	{
	public:
		virtual void* activateInstance(IComponentContext* context) = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IINSTANCE_ACTIVATOR_H_ */