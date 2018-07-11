#pragma once

#include <utility>

#include "Hypodermic/ITypeAlias.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class TypeAlias : public ITypeAlias
    {
    public:
        explicit TypeAlias(const TypeInfo& typeInfo)
            : m_typeInfo(typeInfo)
        {
        }

        bool operator==(const ITypeAlias& rhs) const override
        {
            const ITypeAlias* self = this;
            if (self == &rhs)
                return true;

            auto rhsTypeAlias = dynamic_cast< const TypeAlias* >(&rhs);
            if (rhsTypeAlias == nullptr)
                return false;

            return m_typeInfo == rhsTypeAlias->m_typeInfo;
        }
        
        std::size_t hashCode() const override
        {
            return std::hash< std::type_index >()(m_typeInfo.intrinsicTypeInfo());
        }

        const TypeInfo& typeInfo() const override
        {
            return m_typeInfo;
        }

        std::string toString() const override
        {
            return m_typeInfo.fullyQualifiedName();
        }

    private:
        TypeInfo m_typeInfo;
    };

} // namespace Hypodermic