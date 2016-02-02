#pragma once

#include <type_traits>


namespace Hypodermic
{

    namespace InstanceLifetimes
    {
        enum InstanceLifetime
        {
            Transient,
            Persistent,
        };
    }

    typedef std::integral_constant< InstanceLifetimes::InstanceLifetime, InstanceLifetimes::Transient > TransientInstance;
    typedef std::integral_constant< InstanceLifetimes::InstanceLifetime, InstanceLifetimes::Persistent > PersistentInstance;

} // namespace Hypodermic