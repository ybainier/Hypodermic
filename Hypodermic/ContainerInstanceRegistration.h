#pragma once

#include "Hypodermic/Container.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class ContainerInstanceRegistration : public IRegistration
    {
    public:
        explicit ContainerInstanceRegistration(const std::shared_ptr< Container >& instance)
            : m_instance(instance)
            , m_instanceType(Utils::getMetaTypeInfo< Container >())
            , m_typeAliases()
        {
        }
    
        const TypeInfo& instanceType() const override
        {
            return m_instanceType;
        }

        const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases() const override
        {
            return m_typeAliases;
        }

        std::function< std::shared_ptr< void >(Container&) > getDependencyFactory(const TypeInfo&) const override
        {
            return nullptr;
        }

        std::shared_ptr< void > activate(Container&, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating Container instance of type " << m_instanceType.fullyQualifiedName());

            std::shared_ptr< void > instance = m_instance.lock();

            auto it = m_typeAliases.find(typeAliasKey);
            if (it != std::end(m_typeAliases) && it->second != nullptr)
            {
                auto&& alignPointersFunc = it->second;
                instance = alignPointersFunc(instance);
            }

            if (instance == nullptr)
                HYPODERMIC_LOG_WARN("Container instance of type " << m_instanceType.fullyQualifiedName() << " is null");

            return instance;
        }

    private:
        std::weak_ptr< Container > m_instance;
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
    };

} // namespace Hypodermic