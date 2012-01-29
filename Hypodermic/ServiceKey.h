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

		friend bool operator==(const ServiceKey& lhs, const ServiceKey& rhs);
		friend std::size_t hash_value(const ServiceKey& serviceKey);

	private:
		std::shared_ptr< Service > service_;
	};

	bool operator==(const ServiceKey& lhs, const ServiceKey& rhs);
	std::size_t hash_value(const ServiceKey& serviceKey);

} // namespace Hypodermic


namespace std
{

    template <>
    class hash< Hypodermic::ServiceKey > : public unary_function< Hypodermic::ServiceKey, size_t >
    {	// hash functor
    public:
        std::size_t operator()(const Hypodermic::ServiceKey& key) const
        {
            return Hypodermic::hash_value(key);
        }
    };

} // namespace std


#endif /* !HYPODERMIC_SERVICE_KEY_H_ */