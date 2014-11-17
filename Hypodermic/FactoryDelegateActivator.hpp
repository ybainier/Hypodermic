#ifdef      HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_H_
# ifndef    HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_HPP_
#  define   HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_HPP_



namespace Hypodermic
{

    template <class T>
    inline FactoryDelegateActivator< T >::FactoryDelegateActivator(const std::type_info& typeInfo, ActivationDelegate activationFunction)
		: InstanceActivator(typeInfo), activationFunction_(activationFunction)
	{
	}

    template <class T>
    inline std::shared_ptr< void > FactoryDelegateActivator< T >::activateInstance(std::shared_ptr< IComponentContext > context)
	{
		return activationFunction_(*context);
	}

} // namespace Hypodermic


# endif /* !HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_HPP_ */
#endif  /* HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_H_ */