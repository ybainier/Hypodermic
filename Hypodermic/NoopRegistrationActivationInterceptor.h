#pragma once

#include "Hypodermic/IRegistrationActivationInterceptor.h"


namespace Hypodermic
{

    class NoopRegistrationActivationInterceptor : public IRegistrationActivationInterceptor
    {
    public:
        void onSourceRegistrationActivated(const std::shared_ptr< void >&) override {}
        void onRegistrationActivated(const std::shared_ptr< void >&, const TypeAliasKey&) override {}
    };

} // namespace Hypodermic