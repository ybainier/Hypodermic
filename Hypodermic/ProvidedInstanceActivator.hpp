#ifdef      HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_
# ifndef    HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_HPP_
#  define   HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_HPP_

#  include <stdexcept>


namespace Hypodermic
{
    template <class T>
    ProvidedInstanceActivator< T >::ProvidedInstanceActivator(std::shared_ptr< T > instance)
        : InstanceActivator(typeid(T)), instance_(instance)
        , activated_(false), disposeInstance_(false)
    {
    }

    template <class T>
    ProvidedInstanceActivator< T >::~ProvidedInstanceActivator()
    {
        if (disposeInstance_)
        {
        }
    }

    template <class T>
    std::shared_ptr< void > ProvidedInstanceActivator< T >::activateInstance(std::shared_ptr< IComponentContext > /* context */)
    {
        if (activated_)
            throw std::logic_error("Instance already activated");

        activated_ = true;

        return instance_;
    }

    template <class T>
    bool ProvidedInstanceActivator< T >::disposeInstance() const
    {
        return disposeInstance_;
    }

    template <class T>
    void ProvidedInstanceActivator< T >::disposeInstance(bool value)
    {
        disposeInstance_ = value;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_HPP_ */
#endif /* HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_ */