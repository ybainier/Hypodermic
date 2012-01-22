#ifndef		HYPODERMIC_ISHARING_LIFETIME_SCOPE_H_
# define	HYPODERMIC_ISHARING_LIFETIME_SCOPE_H_

# include <functional>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/ILifetimeScope.h>


namespace Hypodermic
{

	class ISharingLifetimeScope : public ILifetimeScope
	{
	public:
		virtual ISharingLifetimeScope* rootLifetimeScope() = 0;
		virtual ISharingLifetimeScope* parentLifetimeScope() = 0;

		virtual void* getOrCreateAndShare(const boost::uuids::uuid& id, std::function< void*() > creator) = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ISHARING_LIFETIME_SCOPE_H_ */