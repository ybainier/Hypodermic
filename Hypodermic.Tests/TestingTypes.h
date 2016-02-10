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


    class ContainerDependant
    {
    public:
        typedef AutowiredConstructor< ContainerDependant(Container*) > AutowiredSignature;

        ContainerDependant(std::shared_ptr< Container > container)
            : container(container)
        {
            if (this->container == nullptr)
                throw std::runtime_error("Container cannot be null");
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

} // namespace Testing