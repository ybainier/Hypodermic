#pragma once

#include <functional>
#include <memory>

#include <boost/signals2.hpp>

#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class IRegistrationDescriptor;
    class IRegistrationScope;


    class IRegistrationDescriptor
    {
    public:
        typedef boost::signals2::signal< void(const std::shared_ptr< IRegistrationDescriptor >&) > Action;

    public:
        virtual ~IRegistrationDescriptor() {}

        virtual Action& registrationDescriptorUpdated() const = 0;

        virtual std::function< void(IRegistrationScope&) > getDescriptionFactory() const = 0;

        virtual const TypeInfo& instanceType() const = 0;
    };

} // namespace Hypodermic