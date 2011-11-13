#ifndef		HYPODERMIC_SERVICE_KEY_H_
# define	HYPODERMIC_SERVICE_KEY_H_

# include <Hypodermic/Service.h>


namespace Hypodermic
{

	struct ServiceKey
	{
		ServiceKey(Service* service)
			: service_(service)
		{
		}

		friend bool operator==(const ServiceKey& lhs, const ServiceKey& rhs);
		friend std::size_t hash_value(const ServiceKey& serviceKey);

	private:
		Service* service_;
	};

	bool operator==(const ServiceKey& lhs, const ServiceKey& rhs);
	std::size_t hash_value(const ServiceKey& serviceKey);

} // namespace Hypodermic


#endif /* !HYPODERMIC_SERVICE_KEY_H_ */