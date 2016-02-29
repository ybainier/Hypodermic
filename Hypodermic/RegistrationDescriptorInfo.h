#pragma once

#include <string>
#include <type_traits>

#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>

#include "Hypodermic/DependencyFactoryTag.h"
#include "Hypodermic/InstanceLifetime.h"
#include "Hypodermic/InstanceRegistrationTags.h"
#include "Hypodermic/ProvidedDependencyTag.h"
#include "Hypodermic/ProvidedInstanceDependencyTag.h"
#include "Hypodermic/RegistrationDescriptorInfoToString.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    template
    <
        class T,
        InstanceLifetimes::InstanceLifetime Lifetime = InstanceLifetimes::Transient,
        class TInstanceRegistrationTag = Tags::NotSelfRegistered,
        class TRegisteredBases = boost::mpl::map<>,
        class TDependencies = boost::mpl::map<>
    >
    struct RegistrationDescriptorInfo
    {
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
                typename boost::mpl::insert< RegisteredBases, boost::mpl::pair< TBase, boost::mpl::identity< TBase > > >::type,
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
                typename boost::mpl::insert< Dependencies, boost::mpl::pair< TDependency, Tags::DependencyFactory< TDependency > > >::type
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
                typename boost::mpl::insert< Dependencies, boost::mpl::pair< TDependency, Tags::ProvidedInstanceDependency< TDependency, TProvidedDependency > > >::type
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
                typename boost::mpl::insert< Dependencies, boost::mpl::pair< TDependency, Tags::ProvidedDependency< TDependency, TProvidedDependency > > >::type
            >
            Type;
        };

        template <class TDependency>
        struct IsDependencyRegistered : boost::mpl::has_key< Dependencies, TDependency >
        {
        };


        static std::string toString()
        {
            return RegistrationDescriptorInfoToString::toString< RegistrationDescriptorInfo< InstanceType, InstanceLifetime::value, InstanceRegistrationTag, RegisteredBases, Dependencies > >();
        }
    };

} // namespace Hypodermic