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
    std::shared_ptr< void > TypeCaster< ConcreteType, InterfaceType >::cast(std::shared_ptr< void > concreteInstance) const
    {
        std::shared_ptr< ConcreteType > concreteTypeInstance = std::static_pointer_cast< ConcreteType >(concreteInstance);
        std::shared_ptr< InterfaceType > interfaceTypeInstance = concreteTypeInstance;

        return interfaceTypeInstance;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_TYPE_CASTER_HPP_ */
#endif /* HYPODERMIC_TYPE_CASTER_H_ */
