#pragma once

#include <memory>


namespace Hypodermic
{

    class Container;
    class IRegistrationActivationInterceptor;
    class TypeAliasKey;


    class IRegistrationActivator
    {
    public:
        virtual ~IRegistrationActivator() {}

        virtual std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) = 0;

        virtual std::shared_ptr< void > activate(IRegistrationActivationInterceptor& activationInterceptor,
                                                 Container& container,
                                                 const TypeAliasKey& typeAliasKey) = 0;
    };

} // namespace Hypodermic