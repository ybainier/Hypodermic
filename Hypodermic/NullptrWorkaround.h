#ifndef     HYPODERMIC_NULLPTR_WORKAROUND_H_
# define    HYPODERMIC_NULLPTR_WORKAROUND_H_

# include <boost/config.hpp>

# ifdef BOOST_NO_NULLPTR
#  include <memory>

#  define nullptr 0


namespace std
{
    template <class T>
    inline bool operator==(const std::shared_ptr< T >& lhs, int /* rhs */)
    {
        return lhs.get() == nullptr;
    }

    template <class T>
    inline bool operator!=(const std::shared_ptr< T >& lhs, int rhs)
    {
        return !(lhs == rhs);
    }

} // namespace std


# endif /* BOOST_NO_NULLPTR */

#endif  /* !HYPODERMIC_NULLPTR_WORKAROUND_H_ */