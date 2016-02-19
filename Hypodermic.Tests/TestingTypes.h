#pragma once

#include <stdexcept>

#include "Hypodermic/AutowiredConstructor.h"
#include "Hypodermic/Container.h"


using namespace Hypodermic;


namespace Testing
{

    class DefaultConstructibleBase
    {
    public:
        virtual ~DefaultConstructibleBase() {}
    };

    class DefaultConstructible1 : public DefaultConstructibleBase
    {
    };

    class DefaultConstructible2 : public DefaultConstructibleBase
    {
    };


    class ContainerDependent
    {
    public:
        typedef AutowiredConstructor< ContainerDependent(Container*) > AutowiredSignature;

        explicit ContainerDependent(const std::shared_ptr< Container >& container)
            : container(container)
        {
            if (this->container == nullptr)
                throw std::runtime_error("container cannot be null");
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
            if (this->dependency == nullptr)
                throw std::runtime_error("dependency cannot be null");
        }

        std::shared_ptr< MissingConstructorDependency > dependency;
    };



    class ProvidedDependencyBase
    {
    public:
        virtual ~ProvidedDependencyBase() {}
    };

    class ProvidedDependency : public ProvidedDependencyBase
    {
    };

    class AutowiredProvidedConstructor
    {
    public:
        typedef AutowiredConstructor< AutowiredProvidedConstructor(ProvidedDependencyBase*) > AutowiredSignature;

        explicit AutowiredProvidedConstructor(const std::shared_ptr< ProvidedDependencyBase >& dependency)
            : dependency(dependency)
        {
            if (this->dependency == nullptr)
                throw std::runtime_error("dependency cannot be null");
        }

        std::shared_ptr< ProvidedDependencyBase > dependency;
    };

    class NonAutowiredProvidedConstructor
    {
    public:
        explicit NonAutowiredProvidedConstructor(const std::shared_ptr< ProvidedDependencyBase >& dependency)
            : dependency(dependency)
        {
            if (this->dependency == nullptr)
                throw std::runtime_error("dependency cannot be null");
        }

        std::shared_ptr< ProvidedDependencyBase > dependency;
    };

} // namespace Testing