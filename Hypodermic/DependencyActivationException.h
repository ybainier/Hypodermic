#pragma once

#include "Hypodermic/ExceptionBase.h"


namespace Hypodermic
{

    HYPODERMIC_DECLARE_EXCEPTION(DependencyActivationException);

} // namespace Hypodermic


#define HYPODERMIC_THROW_DEPENDENCY_ACTIVATION_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic::DependencyActivationException, message)
