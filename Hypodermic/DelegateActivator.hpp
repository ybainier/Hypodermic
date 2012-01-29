#ifdef      HYPODERMIC_DELEGATE_ACTIVATOR_H_
# ifndef    HYPODERMIC_DELEGATE_ACTIVATOR_HPP_
#  define   HYPODERMIC_DELEGATE_ACTIVATOR_HPP_



namespace Hypodermic
{

    template <class T>
	DelegateActivator< T >::DelegateActivator(const std::type_info& typeInfo, ActivationDelegate activationFunction)
		: InstanceActivator(typeInfo), activationFunction_(activationFunction)
	{
	}

    template <class T>
	std::shared_ptr< void > DelegateActivator< T >::activateInstance(std::shared_ptr< IComponentContext > context)
	{
		return std::shared_ptr< void >(activationFunction_(*context));
	}

} // namespace Hypodermic


# endif /* !HYPODERMIC_DELEGATE_ACTIVATOR_HPP_ */
#endif  /* HYPODERMIC_DELEGATE_ACTIVATOR_H_ */