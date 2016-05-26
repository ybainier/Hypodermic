#pragma once

#include <vector>

#include "Hypodermic/ActivatedRegistrationInfo.h"
#include "Hypodermic/ResolutionInfo.h"


namespace Hypodermic
{

    class ComponentContext;

    
    class ResolutionContext
    {
    public:
        typedef std::vector< ResolutionInfo > ResolutionStack;
        typedef std::vector< ActivatedRegistrationInfo > ActivatedRegistrations;

    public:
        explicit ResolutionContext(ComponentContext& componentContext)
            : m_componentContext(componentContext)
        {
        }

        ComponentContext& componentContext() const
        {
            return m_componentContext;
        }

        ResolutionStack& resolutionStack()
        {
            return m_resolutionStack;
        }

        ActivatedRegistrations& activatedRegistrations()
        {
            return m_activatedRegistrations;
        }

    private:
        ComponentContext& m_componentContext;
        ResolutionStack m_resolutionStack;
        ActivatedRegistrations m_activatedRegistrations;
    };

} // namespace Hypodermic