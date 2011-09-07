#ifndef		TYPED_SERVICE_H_
# define	TYPED_SERVICE_H_

# include <typeinfo>
# include "Service.h"


namespace Hypodermic
{

	class TypedService : public Service
	{
	public:
		TypedService(const type_info& serviceTypeInfo)
			: serviceTypeInfo_(serviceTypeInfo)
		{
		}

		const type_info& serviceTypeInfo() const
		{
			return serviceTypeInfo_;
		}

		bool operator==(const Service& rhs) const
		{
			const TypedService* that = static_cast< const TypedService* >(&rhs);

			if (that == nullptr)
				return false;

			return serviceTypeInfo_ == that->serviceTypeInfo_;
		}

		std::size_t hashValue()
		{
			return boost::hash< const char* >()(serviceTypeInfo_.name());
		}

	private:
		const type_info& serviceTypeInfo_;
	};

} // namespace Hypodermic


#endif /* !TYPED_SERVICE_H_ */