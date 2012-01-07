#ifdef      HYPODERMIC_CONTAINER_BUILDER_H_
# ifndef    HYPODERMIC_CONTAINER_BUILDER_HPP_
#  define   HYPODERMIC_CONTAINER_BUILDER_HPP_

# include <Hypodermic/IContainer.h>


namespace Hypodermic
{

    template <class T>
    boost::shared_ptr< IRegistration< T > > ContainerBuilder::registerConcreteType()
    {
        auto registration = boost::shared_ptr< IRegistration< T > >();

        containerConfigurationCallbacks_.push_back(
            [](Container& c)
            {

            });

        return registration;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_CONTAINER_BUILDER_HPP_ */
#endif  /* HYPODERMIC_CONTAINER_BUILDER_H_ */