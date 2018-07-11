#pragma once

#include <cstdint>
#include <string>


namespace Hypodermic
{

    struct TypeInfo;


    class ITypeAlias
    {
    public:
        virtual ~ITypeAlias() = default;

        virtual bool operator==(const ITypeAlias& rhs) const = 0;
        
        virtual std::size_t hashCode() const = 0;

        virtual const TypeInfo& typeInfo() const = 0;

        virtual std::string toString() const = 0;
    };

} // namespace Hypodermic