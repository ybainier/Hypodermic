#pragma once

#include <memory>


namespace Hypodermic
{

    class TypeAliasKey;


    class IRegistrationActivationInterceptor
    {
    public:
        virtual ~IRegistrationActivationInterceptor() {}

        virtual void onSourceRegistrationActivated(const std::shared_ptr< void >& instance) = 0;
        virtual void onRegistrationActivated(const std::shared_ptr< void >& instance, const TypeAliasKey& typeAliasKey) = 0;
    };

} // namespace Hypodermic