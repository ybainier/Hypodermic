#pragma once

#include <memory>


namespace Hypodermic
{

    struct ActivationResult
    {
        ActivationResult()
            : activated(false)
        {
        }

        bool activated;
        std::shared_ptr< void > activatedInstance;
        std::shared_ptr< void > alignedInstance;
    };
    
} // namespace Hypodermic