#pragma once

#include <memory>


namespace Hypodermic
{

    class ResolutionContext;
    class IRegistration;
    class TypeAliasKey;

    
    class IResolutionContainer
    {
    public:
        virtual ~IResolutionContainer() = default;

        virtual std::shared_ptr< void > getOrCreateComponent(const TypeAliasKey& typeAliasKey,
                                                             const std::shared_ptr< IRegistration >& registration,
                                                             ResolutionContext& resolutionContext) = 0;
    };

} // namespace Hypodermic