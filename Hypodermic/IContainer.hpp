#ifdef      HYPODERMIC_ICONTAINER_H_
# ifndef    HYPODERMIC_ICONTAINER_HPP_
#  define   HYPODERMIC_ICONTAINER_HPP_


namespace Hypodermic
{

    template <class T>
    T* IContainer::resolve()
    {
        return static_cast< T* >(resolve(typeid(T)));
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_ICONTAINER_HPP_ */
#endif  /* HYPODERMIC_ICONTAINER_H_ */