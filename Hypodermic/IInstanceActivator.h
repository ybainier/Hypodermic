#ifndef		HYPODERMIC_IINSTANCE_ACTIVATOR_H_
# define	HYPODERMIC_IINSTANCE_ACTIVATOR_H_

# include <typeinfo>


namespace Hypodermic
{
	class IComponentContext;


	class IInstanceActivator
	{
	public:
		virtual void* activateInstance(IComponentContext* context) = 0;

        virtual const std::type_info& typeInfo() = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IINSTANCE_ACTIVATOR_H_ */