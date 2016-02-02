#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include "Hypodermic2/ITypeAlias.h"
#include "Hypodermic2/TypeAlias.h"
#include "Hypodermic2/TypeInfo.h"


namespace Hypodermic2
{

    class TypeAliasKey
    {
    public:
        explicit TypeAliasKey(const std::shared_ptr< ITypeAlias >& typeAlias)
            : m_typeAlias(typeAlias)
        {
        }

        bool operator==(const TypeAliasKey& rhs) const
        {
            return *m_typeAlias == *rhs.m_typeAlias;
        }

        std::size_t hashCode() const
        {
            return m_typeAlias->hashCode();
        }

        const ITypeAlias& typeAlias() const
        {
            return *m_typeAlias;
        }

    private:
        std::shared_ptr< ITypeAlias > m_typeAlias;
    };


    template <class T>
    inline TypeAliasKey createKeyForType()
    {
        return createKeyForType(Utils::getMetaTypeInfo< T >());
    }

    inline TypeAliasKey createKeyForType(const TypeInfo& typeInfo)
    {
        return TypeAliasKey(std::make_shared< TypeAlias >(typeInfo));
    }

} // namespace Hypodermic2


namespace std
{

    template <>
    struct hash< Hypodermic2::TypeAliasKey > : unary_function< Hypodermic2::TypeAliasKey, size_t >
    {
        size_t operator()(const Hypodermic2::TypeAliasKey& key) const
        {
            return key.hashCode();
        }
    };

} // namespace std