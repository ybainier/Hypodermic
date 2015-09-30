#ifndef		HYPODERMIC_SERVICE_KEY_H_
# define	HYPODERMIC_SERVICE_KEY_H_

# include <functional>
# include <memory>


namespace Hypodermic
{
    class Service;


    struct ServiceKey
    {
        ServiceKey(std::shared_ptr< Service > service);

        bool operator==(const ServiceKey& rhs) const;
        std::size_t hashValue() const;

    private:
        std::shared_ptr< Service > service_;
    };

} // namespace Hypodermic


namespace std
{

    template <>
    struct hash< Hypodermic::ServiceKey > : public unary_function< Hypodermic::ServiceKey, size_t >
    {
    public:
        size_t operator()(const Hypodermic::ServiceKey& key) const
        {
            return key.hashValue();
        }
    };

} // namespace std


#endif /* !HYPODERMIC_SERVICE_KEY_H_ */