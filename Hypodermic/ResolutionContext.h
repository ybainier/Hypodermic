#pragma once

#include "Hypodermic/ActivationStack.h"


namespace Hypodermic
{

    class ComponentContext;

    
    class ResolutionContext
    {
    public:
        ResolutionContext(ComponentContext& componentContext, ActivationStack& activationStack, ActivationStack& activatedRegistrations)
            : m_componentContext(componentContext)
            , m_activationStack(activationStack)
            , m_activatedRegistrations(activatedRegistrations)
        {
        }

        ComponentContext& componentContext() const
        {
            return m_componentContext;
        }

        ActivationStack& activationStack() const
        {
            return m_activationStack;
        }

        ActivationStack& activatedRegistrations() const
        {
            return m_activatedRegistrations;
        }

    public:
        ComponentContext& m_componentContext;
        ActivationStack& m_activationStack;
        ActivationStack& m_activatedRegistrations;
    };

} // namespace Hypodermic