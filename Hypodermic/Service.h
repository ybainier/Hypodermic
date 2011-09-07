#ifndef		SERVICE_H_
# define	SERVICE_H_

# include <boost/functional/hash.hpp>


namespace Hypodermic
{

	class Service
	{
	public:
		virtual bool operator==(const Service& rhs) const = 0;
		
		bool operator!=(const Service& rhs) const
		{
			return !(*this == rhs);
		}

		virtual std::size_t hashValue() = 0;
	};

} // namespace Hypodermic


#endif /* !SERVICE_H_ */