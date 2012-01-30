#ifndef     HYPODERMIC_AUTOWIRED_TYPE_H_
# define    HYPODERMIC_AUTOWIRED_TYPE_H_

# include <type_traits>

# include <boost/mpl/push_back.hpp>
# include <boost/mpl/vector.hpp>

# include <Hypodermic/AutowiredConstructor.h>


namespace Hypodermic
{

    template <class AutowiredConstructorT, class Services = boost::mpl::vector<> >
    struct AutowiredType
    {
        typedef AutowiredConstructorT ConstructorFactory;
        typedef typename AutowiredConstructorT::ConstructibleType ConstructibleType;
        typedef AutowiredType< AutowiredConstructorT, Services > Registration;
        typedef Services Services;

        template <class ServiceT>
        struct As
        {
            static_assert(std::is_same< ConstructibleType, ServiceT >::value || std::is_convertible< ConstructibleType, ServiceT >::value,
                          "AutowiredType< AutowiredConstructor< ConstructibleType > >::As< ServiceT >() requires ConstructibleType to be convertible to ServiceT");

            typedef typename AutowiredType
                        <
                            AutowiredConstructorT,
                            typename boost::mpl::push_back< Services, ServiceT >::type
                        >::Registration Registration;
        };
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_AUTOWIRED_TYPE_H_ */