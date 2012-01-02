#ifdef      HYPODERMIC_TYPE_CASTER_H_
# ifndef    HYPODERMIC_TYPE_CASTER_HPP_
#  define   HYPODERMIC_TYPE_CASTER_HPP_


namespace Hypodermic
{

    template <class ConcreteType, class InterfaceType>
    TypeCaster< ConcreteType, InterfaceType >::TypeCaster(const std::type_info& typeInfo)
        : typeInfo_(typeInfo)
    {
    }

    template <class ConcreteType, class InterfaceType>
    const std::type_info& TypeCaster< ConcreteType, InterfaceType >::typeInfo() const
    {
        return typeInfo_;
    }

    template <class ConcreteType, class InterfaceType>
    void* TypeCaster< ConcreteType, InterfaceType >::cast(void* concreteInstance) const
    {
        ConcreteType concreteTypeInstance = static_cast< ConcreteType >(concreteInstance);
        InterfaceType interfaceTypeInstance = concreteTypeInstance;

        return interfaceTypeInstance;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_TYPE_CASTER_HPP_ */
#endif /* HYPODERMIC_TYPE_CASTER_H_ */
