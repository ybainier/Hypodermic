#include <boost/uuid/random_generator.hpp>

#include "IComponentRegistration.h"
#include "SingleRegistrationStyle.h"


namespace Hypodermic
{

    SingleRegistrationStyle::SingleRegistrationStyle()
        : id_(boost::uuids::random_generator()())
        , preserveDefaults_(false)
        , target_(nullptr)
    {
    }

    const boost::uuids::uuid& SingleRegistrationStyle::id() const
    {
        return id_;
    }

    void SingleRegistrationStyle::id(const boost::uuids::uuid& value)
    {
        id_ = value;
    }

    bool SingleRegistrationStyle::preserveDefaults() const
    {
        return preserveDefaults_;
    }

    void SingleRegistrationStyle::preserveDefaults(bool value)
    {
        preserveDefaults_ = value;
    }

    IComponentRegistration* SingleRegistrationStyle::target()
    {
        return target_;
    }

    void SingleRegistrationStyle::target(IComponentRegistration* value)
    {
        target_ = value;
    }

} // namespace Hypodermic