#ifndef     BOOST_UUID_HASH_FUNCTOR
# define    BOOST_UUID_HASH_FUNCTOR

# include <xfunctional>
# include <boost/uuid/uuid.hpp>


namespace std
{

    template <>
    class hash< boost::uuids::uuid > : public unary_function< boost::uuids::uuid, size_t >
    {	// hash functor
    public:
        size_t operator()(const boost::uuids::uuid& _Keyval) const
        {	// hash _Keyval to size_t value by pseudorandomizing transform
            return boost::uuids::hash_value(_Keyval);
        }
    };

} // namespace std


#endif /* !BOOST_UUID_HASH_FUNCTOR */