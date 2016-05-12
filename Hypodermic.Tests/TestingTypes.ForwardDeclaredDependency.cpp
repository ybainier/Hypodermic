#include "stdafx.h"
#include "TestingTypes.ForwardDeclaredDependency.h"

#include "Hypodermic/ContainerBuilder.h"
#include "TestingTypes.h"


namespace Testing
{
    
    TypeWithOneForwardDeclaredDependency::TypeWithOneForwardDeclaredDependency(const std::shared_ptr< ForwardDeclaredDependency >& dependency)
        : dependency(dependency)
    {
    }

    void TypeWithOneForwardDeclaredDependency::registerDependency(Hypodermic::ContainerBuilder& builder)
    {
        builder.registerType< ForwardDeclaredDependency >();
    }

} // namespace Testing