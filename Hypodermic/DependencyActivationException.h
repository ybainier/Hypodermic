#pragma once

#include "Hypodermic2/ExceptionBase.h"


namespace Hypodermic2
{

    HYPODERMIC_DECLARE_EXCEPTION(DependencyActivationException);

} // namespace Hypodermic2


#define HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic2::DependencyActivationException, message)
