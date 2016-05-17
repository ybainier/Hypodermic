#pragma once

#include <functional>
#include <memory>

#include <boost/signals2.hpp>


namespace Hypodermic
{

    class IRegistrationDescriptor;
    class IRegistrationScope;

    struct TypeInfo;


    class IRegistrationDescriptor
    {
    public:
        typedef boost::signals2::signal< void(const std::shared_ptr< IRegistrationDescriptor >&) > Updated;

    public:
        virtual ~IRegistrationDescriptor() {}

        virtual Updated& registrationDescriptorUpdated() const = 0;

        virtual std::function< void(IRegistrationScope&) > getDescriptionFactory() const = 0;

        virtual const TypeInfo& instanceType() const = 0;
    };

} // namespace Hypodermic