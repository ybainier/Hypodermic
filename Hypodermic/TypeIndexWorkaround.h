#ifndef     HYPODERMIC_TYPEINDEX_WORKAROUND_H_
# define    HYPODERMIC_TYPEINDEX_WORKAROUND_H_

# include <boost/config.hpp>

# ifdef BOOST_NO_0X_HDR_TYPEINDEX
#  include <typeinfo>
#  include <boost/functional/hash.hpp>


namespace std
{
    class type_index
    {
    public:
        type_index(const type_info& typeInfo)
            : typeInfo_(&typeInfo)
        {
        }

        size_t hash_code() const
        {
            const std::string key = name();
            return boost::hash< std::string >()(key);
        }

        const char* name() const
        {
            return typeInfo_->name();
        }

        bool operator==(const type_index& rhs) const
        {
            return *typeInfo_ == *rhs.typeInfo_;
        }

        bool operator!=(const type_index& rhs) const
        {
            return !(*this == rhs);
        }

    private:
        const type_info* typeInfo_;
    };


    template <>
    class hash< type_index > : public std::unary_function< type_index, size_t >
    {	// hash functor
    public:
        std::size_t operator()(const type_index& _Keyval) const
        {
            return _Keyval.hash_code();
        }
    };

} // namespace std


# else

# include <typeindex>

# endif /* BOOST_NO_0X_HDR_TYPEINDEX */

#endif  /* !HYPODERMIC_TYPEINDEX_WORKAROUND_H_ */