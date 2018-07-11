#pragma once

#include <memory>


namespace Hypodermic
{

    class IRegistration;
    class TypeAliasKey;

    
    class ResolutionInfo
    {
    public:
        ResolutionInfo(const std::shared_ptr< IRegistration >& registration, const TypeAliasKey& typeAliasKey)
            : m_registration(registration)
            , m_typeAliasKey(typeAliasKey)
        {
        }

        const std::shared_ptr< IRegistration >& registration() const
        {
            return m_registration;
        }

        const TypeAliasKey& typeAliasKey() const
        {
            return m_typeAliasKey;
        }

    private:
        const std::shared_ptr< IRegistration >& m_registration;
        TypeAliasKey m_typeAliasKey;
    };

} // namespace Hypodermic