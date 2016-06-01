#pragma once

#include <vector>

#include "Hypodermic/ActivatedRegistrationInfo.h"
#include "Hypodermic/IResolutionContext.h"
#include "Hypodermic/ResolutionInfo.h"


namespace Hypodermic
{
    
    class ResolutionContext : public IResolutionContext
    {
    public:
        typedef std::vector< ResolutionInfo > ResolutionStack;
        typedef std::vector< ActivatedRegistrationInfo > ActivatedRegistrations;

    public:
        explicit ResolutionContext(ComponentContext& componentContext, const std::shared_ptr< Container >& container)
            : m_componentContext(componentContext)
            , m_container(container)
        {
        }

        ComponentContext& componentContext() const override
        {
            return m_componentContext;
        }

        const std::weak_ptr< Container >& container() const override
        {
            return m_container;
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
        std::weak_ptr< Container > m_container;
        ResolutionStack m_resolutionStack;
        ActivatedRegistrations m_activatedRegistrations;
    };

} // namespace Hypodermic