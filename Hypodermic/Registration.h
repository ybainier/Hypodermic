#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <boost/signals2.hpp>

#include "Hypodermic/CircularDependencyException.h"
#include "Hypodermic/DependencyActivationException.h"
#include "Hypodermic/InstanceAlreadyActivatingException.h"
#include "Hypodermic/InvokeAtScopeExit.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container;


    class Registration : public IRegistration
    {
    public:
        Registration(const TypeInfo& instanceType,
                     const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases,
                     const std::function< std::shared_ptr< void >(Container&) >& instanceFactory,
                     const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories,
                     const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
            : m_instanceType(instanceType)
            , m_typeAliases(typeAliases)
            , m_instanceFactory(instanceFactory)
            , m_dependencyFactories(dependencyFactories)
            , m_activating(false)
        {
            for (auto&& handler : activationHandlers)
                m_activated.connect(handler);
        }

        const TypeInfo& instanceType() const override
        {
            return m_instanceType;
        }

        const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases() const override
        {
            return m_typeAliases;
        }

        std::function< std::shared_ptr< void >(Container&) > getDependencyFactory(const TypeInfo& dependencyType) const override
        {
            auto factoryIt = m_dependencyFactories.find(dependencyType);
            if (factoryIt == std::end(m_dependencyFactories))
                return nullptr;

            return factoryIt->second;
        }

        std::shared_ptr< void > activate(Container& container, const TypeAliasKey& typeAliasKey) override
        {
            HYPODERMIC_LOG_INFO("Activating type " << instanceType().fullyQualifiedName());

            Utils::InvokeAtScopeExit atScopeExit([this]() { this->m_activating = false; });

            auto&& typeInfo = typeAliasKey.typeAlias().typeInfo();

            {
                std::lock_guard< decltype(m_mutex) > lock(m_mutex);

                if (m_activating)
                {
                    if (m_instanceType == typeInfo)
                    {
                        HYPODERMIC_LOG_ERROR("Already activating '" << typeInfo.fullyQualifiedName() << "', unwinding...");

                        HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION("Already activating '" << typeInfo.fullyQualifiedName() << "', unwinding...");
                    }
                    else
                    {
                        HYPODERMIC_LOG_ERROR("Already activating '" << typeInfo.fullyQualifiedName() << "' (base of '" << m_instanceType.fullyQualifiedName() << "'), unwinding...");

                        HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION("Already activating '" << typeInfo.fullyQualifiedName() <<
                                                                               "' (base of '" << m_instanceType.fullyQualifiedName() << "'), unwinding...");
                    }
                }

                m_activating = true;
            }

            if (!m_instanceFactory)
            {
                HYPODERMIC_LOG_WARN("No instance factory provided to activate type " << m_instanceType.fullyQualifiedName());
                return nullptr;
            }

            try
            {
                auto&& instance = m_instanceFactory(container);

                m_activated(container, instance);

                auto it = m_typeAliases.find(typeAliasKey);
                if (it != std::end(m_typeAliases) && it->second != nullptr)
                {
                    auto&& alignPointersFunc = it->second;
                    instance = alignPointersFunc(instance);
                }

                if (instance == nullptr)
                    HYPODERMIC_LOG_WARN("Instance of type " << m_instanceType.fullyQualifiedName() << " is null");

                return instance;
            }
            catch (CircularDependencyException& ex)
            {
                HYPODERMIC_LOG_ERROR("Circular dependency detected while activating type " << m_instanceType.fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION("'" << m_instanceType.fullyQualifiedName() << "' -> " << ex.what());
            }
            catch (InstanceAlreadyActivatingException& ex)
            {
                HYPODERMIC_LOG_ERROR("Circular dependency detected while activating type " << m_instanceType.fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION("'" << m_instanceType.fullyQualifiedName() << "': " << ex.what());
            }
            catch (DependencyActivationException& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << m_instanceType.fullyQualifiedName() << " because one dependency cannot be activated: " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << m_instanceType.fullyQualifiedName() << " because one dependency cannot be activated: " << ex.what());
            }
            catch (std::exception& ex)
            {
                HYPODERMIC_LOG_ERROR("Unable to activate instance of type " << m_instanceType.fullyQualifiedName() << ": " << ex.what());

                HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION("Unable to activate instance of type " << m_instanceType.fullyQualifiedName() << ": " << ex.what());
            }
        }

    private:
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
        std::function< std::shared_ptr< void >(Container&) > m_instanceFactory;
        std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > > m_dependencyFactories;
        boost::signals2::signal< void(Container&, const std::shared_ptr< void >&) > m_activated;
        bool m_activating;
        std::recursive_mutex m_mutex;
    };

} // namespace Hypodermic