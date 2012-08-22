#ifndef		HYPODERMIC_SERVICE_H_
# define	HYPODERMIC_SERVICE_H_

# include <cstddef>
# include <typeinfo>


namespace Hypodermic
{

    class Service
    {
    public:
        virtual ~Service() = 0;

        virtual const std::type_info& typeInfo() const = 0;

        virtual bool operator==(const Service& rhs) const = 0;
        virtual std::size_t hashValue() const = 0;
    };

    inline Service::~Service() {}

} // namespace Hypodermic


#endif /* !HYPODERMIC_SERVICE_H_ */