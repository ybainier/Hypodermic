#ifndef     HYPODERMIC_CONTAINER_BUILDER_H_
# define    HYPODERMIC_CONTAINER_BUILDER_H_

# include <vector>
# include <boost/function.hpp>
# include <boost/shared_ptr.hpp>

# include <Hypodermic/IRegistration.h>


namespace Hypodermic
{
    class Container;
    class IContainer;


    class ContainerBuilder
    {
    public:
        template <class T> boost::shared_ptr< IRegistration< T > > registerConcreteType();

        boost::shared_ptr< IContainer > build();

    private:
        std::vector< boost::function< void(Container&) > > containerConfigurationCallbacks_;
    };

} // namespace Hypodermic


# include <Hypodermic/ContainerBuilder.hpp>

#endif /* !HYPODERMIC_CONTAINER_BUILDER_H_ */