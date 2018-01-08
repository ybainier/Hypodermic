#pragma once

#include "Hypodermic/DependencyFactory.h"
#include "Hypodermic/InstanceLifetime.h"
#include "Hypodermic/TypeAliases.h"


namespace Hypodermic
{

    class Container;
    class IRegistrationActivator;
    struct TypeInfo;


    class IRegistration
    {
    public:
        virtual ~IRegistration() = default;

        virtual const TypeInfo& instanceType() const = 0;

        virtual const TypeAliases& typeAliases() const = 0;

        virtual DependencyFactory getDependencyFactory(const TypeInfo& dependencyType) const = 0;

        virtual IRegistrationActivator& activator() const = 0;

        virtual InstanceLifetimes::InstanceLifetime instanceLifetime() const = 0;

        virtual bool isFallback() const = 0;
    };

} // namespace Hypodermic