#pragma once

#include <memory>

namespace Hypodermic { class ContainerBuilder; }


namespace Testing
{

    class ForwardDeclaredDependency;


    class TypeWithOneForwardDeclaredDependency
    {
    public:
        explicit TypeWithOneForwardDeclaredDependency(const std::shared_ptr< ForwardDeclaredDependency >& dependency);

        static void registerDependency(Hypodermic::ContainerBuilder& builder);

        std::shared_ptr< ForwardDeclaredDependency > dependency;
    };
    
} // namespace Testing