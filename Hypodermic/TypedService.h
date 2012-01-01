#ifndef		HYPODERMIC_TYPED_SERVICE_H_
# define	HYPODERMIC_TYPED_SERVICE_H_

# include <typeinfo>

# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class TypedService : public Service
	{
	public:
		TypedService(const type_info& serviceTypeInfo);

		const type_info& serviceTypeInfo() const;

		bool operator==(const Service& rhs) const;

		std::size_t hashValue();

	private:
		const type_info& serviceTypeInfo_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_TYPED_SERVICE_H_ */