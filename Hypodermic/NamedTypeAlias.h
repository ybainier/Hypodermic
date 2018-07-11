#pragma once

#include <string>
#include <utility>

#include "Hypodermic/ITypeAlias.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class NamedTypeAlias : public ITypeAlias
    {
    public:
        explicit NamedTypeAlias(const TypeInfo& typeInfo, const std::string& name)
            : m_typeInfo(typeInfo)
            , m_name(name)
        {
        }

        bool operator==(const ITypeAlias& rhs) const override
        {
            const ITypeAlias* self = this;
            if (self == &rhs)
                return true;

            auto rhsTypeAlias = dynamic_cast< const NamedTypeAlias* >(&rhs);
            if (rhsTypeAlias == nullptr)
                return false;

            return m_typeInfo == rhsTypeAlias->m_typeInfo && m_name == rhsTypeAlias->m_name;
        }
        
        std::size_t hashCode() const override
        {
            auto hashCode = std::hash< std::type_index >()(m_typeInfo.intrinsicTypeInfo());
            return (hashCode * 397) ^ std::hash< std::string >()(m_name);
        }

        const TypeInfo& typeInfo() const override
        {
            return m_typeInfo;
        }

        std::string toString() const override
        {
            return m_typeInfo.fullyQualifiedName() + " (named '" + m_name + "')";
        }

    private:
        TypeInfo m_typeInfo;
        std::string m_name;
    };

} // namespace Hypodermic