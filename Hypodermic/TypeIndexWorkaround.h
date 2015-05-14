#ifndef     HYPODERMIC_TYPEINDEX_WORKAROUND_H_
# define    HYPODERMIC_TYPEINDEX_WORKAROUND_H_

# include <boost/config.hpp>

# ifdef BOOST_NO_0X_HDR_TYPEINDEX
#  include <functional>
#  include <typeinfo>


namespace std
{
    class type_index
    {
    public:
        type_index(const type_info& typeInfo)
            : typeInfo_(&typeInfo)
            , key_(typeInfo_->name())
        {
        }

        size_t hash_code() const
        {
            return std::hash< std::string >()(key_);
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
        const std::string key_;
    };


    template <>
    class hash< type_index > : public unary_function< type_index, size_t >
    {
    public:
        size_t operator()(const type_index& key) const
        {
            return key.hash_code();
        }
    };

} // namespace std


# else

# include <typeindex>

# endif /* BOOST_NO_0X_HDR_TYPEINDEX */

#endif  /* !HYPODERMIC_TYPEINDEX_WORKAROUND_H_ */