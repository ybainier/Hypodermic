#ifndef		HYPODERMIC_SERVICE_KEY_H_
# define	HYPODERMIC_SERVICE_KEY_H_


namespace Hypodermic
{
    class Service;


	struct ServiceKey
	{
		ServiceKey(Service* service);

		friend bool operator==(const ServiceKey& lhs, const ServiceKey& rhs);
		friend std::size_t hash_value(const ServiceKey& serviceKey);

	private:
		Service* service_;
	};

	bool operator==(const ServiceKey& lhs, const ServiceKey& rhs);
	std::size_t hash_value(const ServiceKey& serviceKey);

} // namespace Hypodermic


#endif /* !HYPODERMIC_SERVICE_KEY_H_ */