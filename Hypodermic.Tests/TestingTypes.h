#pragma once

#include <stdexcept>

#include "Hypodermic/Container.h"
#include "Hypodermic/FactoryWrapper.h"


namespace Hypodermic
{
namespace Testing
{

    class DefaultConstructibleBase
    {
    public:
        virtual ~DefaultConstructibleBase() = default;

        virtual void doSomething() = 0;
    };

    class DefaultConstructible1 : public DefaultConstructibleBase
    {
    public:
        void doSomething() override {}
    };

    class DefaultConstructible2 : public DefaultConstructibleBase
    {
    public:
        void doSomething() override {}
    };

    class DefaultConstructible3 : public DefaultConstructibleBase
    {
    public:
        void doSomething() override {}
    };


    class ContainerDependent
    {
    public:
        explicit ContainerDependent(const std::shared_ptr< Container >& container)
            : container(container)
        {
        }

        std::shared_ptr< Container > container;
    };


    class ManualConstructible
    {
    public:
        explicit ManualConstructible(int i)
            : i(i)
        { }

        int i;
    };



    class MissingConstructorDependency
    {
    };

    class MissingConstructor
    {
    public:
        explicit MissingConstructor(const std::shared_ptr< MissingConstructorDependency >& dependency)
            : dependency(dependency)
        {
        }

        std::shared_ptr< MissingConstructorDependency > dependency;
    };



    class ProvidedDependencyBase
    {
    public:
        virtual ~ProvidedDependencyBase() = default;

        virtual void doSomething() = 0;
    };

    class ProvidedDependency : public ProvidedDependencyBase
    {
    public:
        void doSomething() override {}
    };

    class RandomTypeBase
    {
    public:
        virtual ~RandomTypeBase() = default;
    };

    class TypeWithOneDependency : public RandomTypeBase
    {
    public:
        explicit TypeWithOneDependency(const std::shared_ptr< ProvidedDependencyBase >& dependency)
            : dependency(dependency)
        {
        }

        std::shared_ptr< ProvidedDependencyBase > dependency;
    };


    class NestedDependency
    {
    public:
        explicit NestedDependency(int i)
            : i(i)
        {
        }

        int i;
    };

    class TopLevelConstructor
    {
    public:
        explicit TopLevelConstructor(const std::shared_ptr< NestedDependency >& dependency)
            : dependency(dependency)
        {
        }

        std::shared_ptr< NestedDependency > dependency;
    };


    class BaseType1
    {
    public:
        virtual ~BaseType1() = default;
    };

    class BaseType2
    {
    public:
        virtual ~BaseType2() = default;
    };

    class Type1 : public BaseType1
    {
    public:
        explicit Type1(const std::shared_ptr< BaseType2 >&)
        {
        }
    };

    class Type2 : public BaseType2
    {
    public:
        explicit Type2(const std::shared_ptr< BaseType1 >&)
        {
        }
    };


    class ILoader
    {
    public:
        virtual ~ILoader() = default;

        virtual void load() = 0;
    };

    class Loader : public ILoader
    {
    public:
        void load() override {}
    };
    

    class TypeThatNeedsLoader
    {
    public:
        explicit TypeThatNeedsLoader(const std::shared_ptr< ILoader >& dependency)
            : dependency(dependency)
        {
        }

        std::shared_ptr< ILoader > dependency;
    };


    class Initializer
    {
    public:
        explicit Initializer(const std::shared_ptr< TypeThatNeedsLoader >& dependency)
            : dependency(dependency)
        {
        }

        std::shared_ptr< TypeThatNeedsLoader > dependency;
    };


    class IncompleteType;
    class CompleteType {};


    class TypeWithInjectedFactory
    {
    public:
        explicit TypeWithInjectedFactory(const std::function< std::shared_ptr< ILoader >() >& loaderFactory)
            : factory(loaderFactory)
        {
        }

        std::function< std::shared_ptr< ILoader >() > factory;
    };

    class TypeWithInjectedFactoryWrapper
    {
    public:
        explicit TypeWithInjectedFactoryWrapper(const FactoryWrapper< ILoader >& factoryWrapper)
            : factory(factoryWrapper.getFactory())
        {
        }

        std::function< std::shared_ptr< ILoader >() > factory;
    };

} // namespace Testing
} // namespace Hypodermic