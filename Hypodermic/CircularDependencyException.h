#pragma once

#include "Hypodermic2/ExceptionBase.h"


namespace Hypodermic2
{

    HYPODERMIC_DECLARE_EXCEPTION(CircularDependencyException);

} // namespace Hypodermic2


#define HYPODERMIC_THROW_CIRCULAR_DEPENDENCY_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic2::CircularDependencyException, message)
