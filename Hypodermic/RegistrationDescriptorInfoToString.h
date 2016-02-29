#pragma once

#include <sstream>
#include <string>
#include <type_traits>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/pair.hpp>

#include "Hypodermic/DependencyFactoryTag.h"
#include "Hypodermic/ProvidedDependencyTag.h"
#include "Hypodermic/ProvidedInstanceDependencyTag.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    namespace Details
    {

        template <class TTag>
        struct TagToString;

        template <class TDependency>
        struct TagToString< Tags::DependencyFactory< TDependency > >
        {
            typedef TDependency Dependency;

            static std::string toString()
            {
                std::stringstream stream;
                stream << ".WithFactoryOf< " <<
                    Utils::getMetaTypeInfo< Dependency >().fullyQualifiedName() << " >";

                return stream.str();
            }
        };

        template <class TDependency, class TProvidedDependency>
        struct TagToString< Tags::ProvidedInstanceDependency< TDependency, TProvidedDependency > >
        {
            typedef TDependency Dependency;
            typedef TProvidedDependency ProvidedDependency;

            static std::string toString()
            {
                std::stringstream stream;
                stream << ".WithInstanceOf< " <<
                    Utils::getMetaTypeInfo< Dependency >().fullyQualifiedName() << ", " <<
                    Utils::getMetaTypeInfo< ProvidedDependency >().fullyQualifiedName() << " >";

                return stream.str();
            }
        };

        template <class TDependency, class TProvidedDependency>
        struct TagToString< Tags::ProvidedDependency< TDependency, TProvidedDependency > >
        {
            typedef TDependency Dependency;
            typedef TProvidedDependency ProvidedDependency;

            static std::string toString()
            {
                std::stringstream stream;
                stream << ".With< " <<
                    Utils::getMetaTypeInfo< Dependency >().fullyQualifiedName() << ", " <<
                    Utils::getMetaTypeInfo< ProvidedDependency >().fullyQualifiedName() << " >";

                return stream.str();
            }
        };

        struct RegisteredBaseToString
        {
            RegisteredBaseToString(std::ostream& stream)
                : m_stream(stream)
            {
            }

            template <class TIdentity>
            void operator()(const TIdentity&)
            {
                m_stream << ".As< " << Utils::getMetaTypeInfo< typename TIdentity::type >().fullyQualifiedName() << " >";
            }

        private:
            std::ostream& m_stream;
        };

        struct DependencyToString
        {
            DependencyToString(std::ostream& stream)
                : m_stream(stream)
            {
            }

            template <class TKey, class TDependencyTag>
            void operator()(const boost::mpl::pair< TKey, TDependencyTag >&) const
            {
                m_stream << TagToString< TDependencyTag >::toString();
            }

        private:
            std::ostream& m_stream;
        };

    } // namespace Details



    struct RegistrationDescriptorInfoToString
    {
        template <class TDescriptorInfo>
        static std::string toString()
        {
            typedef typename TDescriptorInfo::InstanceType InstanceType;
            typedef typename TDescriptorInfo::InstanceRegistrationTag InstanceRegistrationTag;
            typedef typename TDescriptorInfo::InstanceLifetime InstanceLifetime;
            typedef typename TDescriptorInfo::RegisteredBases RegisteredBases;
            typedef typename TDescriptorInfo::Dependencies Dependencies;

            std::stringstream stream;

            stream << "RegistrationOf< " << Utils::getMetaTypeInfo< InstanceType >().fullyQualifiedName() << " >";

            if (std::is_same< InstanceRegistrationTag, Tags::SelfRegistered >::value)
                stream << ".AsSelf";

            if (InstanceLifetime::value == InstanceLifetimes::Persistent)
                stream << ".SingleInstance";

            boost::mpl::for_each< RegisteredBases >(Details::RegisteredBaseToString(stream));
            boost::mpl::for_each< Dependencies >(Details::DependencyToString(stream));

            return stream.str();
        }
    };

} // namespace Hypodermic