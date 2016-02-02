#pragma once

#include <cstdint>


namespace Hypodermic
{

    struct TypeInfo;


    class ITypeAlias
    {
    public:
        virtual ~ITypeAlias() {}

        virtual bool operator==(const ITypeAlias& rhs) const = 0;
        
        virtual std::size_t hashCode() const = 0;

        virtual const TypeInfo& typeInfo() const = 0;
    };

} // namespace Hypodermic