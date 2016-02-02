#pragma once

#include <sstream>
#include <string>
#include <type_traits>

#include <boost/mpl/empty.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/set.hpp>

#include "Hypodermic/InstanceLifetime.h"
#include "Hypodermic/InstanceRegistrationTags.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    template
    <
        class T,
        InstanceLifetimes::InstanceLifetime Lifetime = InstanceLifetimes::Transient,
        class TInstanceRegistrationTag = Tags::NotSelfRegistered,
        class TRegisteredBases = boost::mpl::set<>,
        class TDependencies = boost::mpl::map<>
    >
    struct RegistrationDescriptorInfo
    {
    private:
        template <class TDependency>
        struct DependencyFactoryTag
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
        struct ProvidedInstanceDependencyTag
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
        struct ProvidedDependencyTag
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

    public:
        typedef T InstanceType;
        typedef std::integral_constant< InstanceLifetimes::InstanceLifetime, Lifetime > InstanceLifetime;
        typedef TInstanceRegistrationTag InstanceRegistrationTag;
        typedef TRegisteredBases RegisteredBases;
        typedef TDependencies Dependencies;

        struct SingleInstance
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetimes::Persistent,
                InstanceRegistrationTag,
                RegisteredBases,
                Dependencies
            >
            Type;
        };

        struct SelfRegistered
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                Tags::SelfRegistered,
                RegisteredBases,
                Dependencies
            >
            Type;
        };

        template <class TBase>
        struct RegisterBase
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                InstanceRegistrationTag,
                typename boost::mpl::insert< RegisteredBases, TBase >::type,
                Dependencies
            >
            Type;
        };

        template <class TBase>
        struct IsBaseRegistered : boost::mpl::has_key< RegisteredBases, TBase >
        {
        };

        template <class TDependency>
        struct RegisterDependencyFactory
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                InstanceRegistrationTag,
                RegisteredBases,
                typename boost::mpl::insert< Dependencies, boost::mpl::pair< TDependency, DependencyFactoryTag< TDependency > > >::type
            >
            Type;
        };

        template <class TDependency, class TProvidedDependency>
        struct RegisterDependencyInstance
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                InstanceRegistrationTag,
                RegisteredBases,
                typename boost::mpl::insert< Dependencies, boost::mpl::pair< TDependency, ProvidedInstanceDependencyTag< TDependency, TProvidedDependency > > >::type
            >
            Type;
        };

        template <class TDependency, class TProvidedDependency>
        struct RegisterDependency
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                InstanceRegistrationTag,
                RegisteredBases,
                typename boost::mpl::insert< Dependencies, boost::mpl::pair< TDependency, ProvidedDependencyTag< TDependency, TProvidedDependency > > >::type
            >
            Type;
        };

        template <class TDependency>
        struct IsDependencyRegistered : boost::mpl::has_key< Dependencies, TDependency >
        {
        };


    private:
        struct RegisteredBaseToString
        {
            RegisteredBaseToString(std::ostream& stream)
                : m_stream(stream)
            {
            }

            template <class T>
            void operator()(const T&)
            {
                m_stream << ".As< " << Utils::getMetaTypeInfo< T >().fullyQualifiedName() << " >";
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
                m_stream << TDependencyTag::toString();
            }

        private:
            std::ostream& m_stream;
        };

    public:
        static std::string toString()
        {
            std::stringstream stream;

            stream << "RegistrationOf< " << Utils::getMetaTypeInfo< InstanceType >().fullyQualifiedName() << " >";

            if (std::is_same< InstanceRegistrationTag, Tags::SelfRegistered >::value)
                stream << ".AsSelf";

            if (InstanceLifetime::value == InstanceLifetimes::Persistent)
                stream << ".SingleInstance";

            boost::mpl::for_each< RegisteredBases >(RegisteredBaseToString(stream));
            boost::mpl::for_each< Dependencies >(DependencyToString(stream));

            return stream.str();
        }
    };

} // namespace Hypodermic