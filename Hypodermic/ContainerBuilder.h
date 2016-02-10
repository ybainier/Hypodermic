#pragma once

#include <functional>
#include <memory>

#include "Hypodermic/Container.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/RegistrationDescriptorBuilder.h"


namespace Hypodermic
{

    class ContainerBuilder
    {
    public:
        /// <summary>
        /// Register type T
        /// </summary>
        /// <param name="T">The type to register inside ContainerBuilder</param>
        /// <returns>A reference on a new IRegistrationDescriptor</returns>
        template <class T>
        typename RegistrationDescriptorBuilder::ForTypeConstruction< T >::Type& registerType()
        {
            typedef typename RegistrationDescriptorBuilder::ForTypeConstruction< T >::Type RegistrationDescriptorType;

            return finalizeRegistration(std::make_shared< RegistrationDescriptorType >());
        }

        /// <summary>
        /// Register a shared instance of type T
        /// </summary>
        /// <param name="instance">The shared instance to register inside ContainerBuilder</param>
        /// <returns>A reference on a new ProvidedInstanceRegistrationDescriptor</returns>
        template <class T>
        typename RegistrationDescriptorBuilder::ForProvidedInstance< T >::Type& registerInstance(const std::shared_ptr< T >& instance)
        {
            typedef typename RegistrationDescriptorBuilder::ForProvidedInstance< T >::Type RegistrationDescriptorType;

            return finalizeRegistration(std::make_shared< RegistrationDescriptorType >(instance));
        }

        /// <summary>
        /// Register a functor producing an instance of T
        /// </summary>
        /// <param name="instanceFactory">The functor to register inside ContainerBuilder</param>
        /// <returns>A reference on a new IRegistrationDescriptor</returns>
        template <class TCallable>
        typename RegistrationDescriptorBuilder::ForProvidedInstanceFactory< TCallable >::Type& registerInstanceFactory(const TCallable& instanceFactory)
        {
            typedef typename RegistrationDescriptorBuilder::ForProvidedInstanceFactory< TCallable >::InstanceType InstanceType;
            typedef typename RegistrationDescriptorBuilder::ForProvidedInstanceFactory< TCallable >::Type RegistrationDescriptorType;

            return finalizeRegistration(std::make_shared< RegistrationDescriptorType >(instanceFactory));
        }

        /// <summary>
        /// Build a new container
        /// </summary>
        /// <returns>A shared pointer to a new Container</returns>
        std::shared_ptr< Container > build()
        {
            HYPODERMIC_LOG_INFO("Building container");

            auto scope = std::make_shared< RegistrationScope >();
            build(*scope);

            auto container = std::make_shared< Container >(scope);
            registerContainerInstance(container, scope);

            return container;
        }

        /// <summary>
        /// Build a new and nested container from a passed Container
        /// </summary>
        /// <param name="container">The Container from which to create a nested Container</param>
        /// <returns>A shared pointer to a new Container</returns>
        std::shared_ptr< Container > buildNestedContainerFrom(const Container& container)
        {
            HYPODERMIC_LOG_INFO("Building nested container");

            auto scope = container.createNestedScope();
            build(*scope);

            auto nestedContainer = std::make_shared< Container >(scope);
            registerContainerInstance(nestedContainer, scope);

            return nestedContainer;
        }

    private:
        void registerContainerInstance(const std::shared_ptr< Container >& container, const std::shared_ptr< IRegistrationScope >& scope)
        {
            typedef typename RegistrationDescriptorBuilder::ForProvidedInstance< Container >::Type RegistrationDescriptorType;

            auto registrationDescriptor = std::make_shared< RegistrationDescriptorType >(container);
            auto factory = registrationDescriptor->getDescriptionFactory();

            factory(*scope);
        }

        void build(IRegistrationScope& scope)
        {
            for (auto&& x : m_registrationDescriptors)
            {
                auto&& action = m_buildActions[x];
                action(scope);
            }
        }

        template <class TRegistrationDescriptor>
        TRegistrationDescriptor& finalizeRegistration(const std::shared_ptr< TRegistrationDescriptor >& registrationDescriptor)
        {
            m_buildActions[registrationDescriptor] = registrationDescriptor->getDescriptionFactory();
            m_registrationDescriptors.push_back(registrationDescriptor);

            listenToRegistrationDescriptorUpdates(registrationDescriptor);

            return *registrationDescriptor;
        }

        void listenToRegistrationDescriptorUpdates(const std::shared_ptr< IRegistrationDescriptor >& registrationDescriptor)
        {
            registrationDescriptor->registrationDescriptorUpdated().connect([this, registrationDescriptor](const std::shared_ptr< IRegistrationDescriptor >& x)
            {
                registrationDescriptor->registrationDescriptorUpdated().disconnect_all_slots();

                m_buildActions.erase(registrationDescriptor);
                m_registrationDescriptors.insert
                (
                    m_registrationDescriptors.erase
                    (
                        std::find(std::begin(m_registrationDescriptors), std::end(m_registrationDescriptors), registrationDescriptor)
                    ),
                    x
                );

                m_buildActions[x] = x->getDescriptionFactory();

                listenToRegistrationDescriptorUpdates(x);
            });
        }

    private:
        std::vector< std::shared_ptr< IRegistrationDescriptor > > m_registrationDescriptors;
        std::unordered_map< std::shared_ptr< IRegistrationDescriptor >, std::function< void(IRegistrationScope&) > > m_buildActions;
    };

} // namespace Hypodermic
