#pragma once

#include <memory>
#include <vector>

#include "Hypodermic/IRegistration.h"


namespace Hypodermic
{

    typedef std::vector< std::shared_ptr< IRegistration > > ActivationStack;

} // namespace Hypodermic