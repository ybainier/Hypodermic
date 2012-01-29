#ifndef     HYPODERMIC_KEYED_SERVICE_H_
# define    HYPODERMIC_KEYED_SERVICE_H_

# include <string>
# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class KeyedService : public Service
	{
	public:
		KeyedService(const std::string& name, const std::type_info& typeInfo);

        const std::string& name() const;
		const std::type_info& typeInfo() const;

        bool operator==(const Service& rhs) const;
        std::size_t hashValue() const;

    private:
        std::string name_;
		const std::type_info& typeInfo_;
	};

} // namespace Hypodermic


#endif /* HYPODERMIC_KEYED_SERVICE_H_ */