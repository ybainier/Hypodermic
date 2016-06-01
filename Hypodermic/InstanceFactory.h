#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class IRegistration;
    class IResolutionContext;

    typedef std::function< std::shared_ptr< void >(const IRegistration&, IResolutionContext&) > InstanceFactory;

} // namespace Hypodermic