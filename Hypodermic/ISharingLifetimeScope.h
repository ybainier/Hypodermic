#ifndef		HYPODERMIC_ISHARING_LIFETIME_SCOPE_H_
# define	HYPODERMIC_ISHARING_LIFETIME_SCOPE_H_

# include <functional>
# include <memory>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/ILifetimeScope.h>


namespace Hypodermic
{

	class ISharingLifetimeScope : public ILifetimeScope
	{
	public:
		virtual ISharingLifetimeScope* rootLifetimeScope() = 0;
		virtual ISharingLifetimeScope* parentLifetimeScope() = 0;

		virtual std::shared_ptr< void > getOrCreateAndShare(const boost::uuids::uuid& id,
                                                            std::function< std::shared_ptr< void >() > creator) = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ISHARING_LIFETIME_SCOPE_H_ */