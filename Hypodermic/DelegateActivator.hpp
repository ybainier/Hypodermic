#ifdef      HYPODERMIC_DELEGATE_ACTIVATOR_H_
# ifndef    HYPODERMIC_DELEGATE_ACTIVATOR_HPP_
#  define   HYPODERMIC_DELEGATE_ACTIVATOR_HPP_



namespace Hypodermic
{

    template <class T>
	inline DelegateActivator< T >::DelegateActivator(const std::type_info& typeInfo, ActivationDelegate activationFunction)
		: InstanceActivator(typeInfo), activationFunction_(activationFunction)
	{
	}

    template <class T>
	inline std::shared_ptr< void > DelegateActivator< T >::activateInstance(std::shared_ptr< IComponentContext > context)
	{
		return std::shared_ptr< void >(activationFunction_(*context));
	}

    inline DelegateActivator< void >::DelegateActivator(const std::type_info& typeInfo, ActivationDelegate activationFunction)
        : InstanceActivator(typeInfo), activationFunction_(activationFunction)
    {
    }

    inline std::shared_ptr< void > DelegateActivator< void >::activateInstance(std::shared_ptr< IComponentContext > context)
    {
        return activationFunction_(*context);
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_DELEGATE_ACTIVATOR_HPP_ */
#endif  /* HYPODERMIC_DELEGATE_ACTIVATOR_H_ */