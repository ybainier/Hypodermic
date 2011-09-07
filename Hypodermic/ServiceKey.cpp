#include "ServiceKey.h"


namespace Hypodermic
{
	bool operator==(const ServiceKey& lhs, const ServiceKey& rhs)
	{
		return *lhs.service_ == *rhs.service_;
	}


	std::size_t hash_value(const ServiceKey& serviceKey)
	{
		return serviceKey.service_->hashValue();
	}

} // namespace Hypodermic