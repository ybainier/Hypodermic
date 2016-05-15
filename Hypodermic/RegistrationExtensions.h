#pragma once

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{

    class RegistrationExtensions
    {
    public:
        static std::shared_ptr< void > getAlignedPointer(const IRegistration& registration,
                                                         const std::shared_ptr< void >& instance,
                                                         const TypeAliasKey& typeAliasKey)
        {
            auto it = registration.typeAliases().find(typeAliasKey);
            if (it == std::end(registration.typeAliases()) || it->second == nullptr)
                return instance;

            auto&& alignPointersFunc = it->second;
            return alignPointersFunc(instance);
        }
    };

} // namespace Hypodermic