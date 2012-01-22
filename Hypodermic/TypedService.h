#ifndef		HYPODERMIC_TYPED_SERVICE_H_
# define	HYPODERMIC_TYPED_SERVICE_H_

# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class TypedService : public Service
	{
	public:
		TypedService(const std::type_info& typeInfo);

		const std::type_info& typeInfo() const;

	private:
		const std::type_info& typeInfo_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_TYPED_SERVICE_H_ */