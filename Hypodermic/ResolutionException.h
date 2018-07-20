#pragma once

#include "Hypodermic/ExceptionBase.h"


namespace Hypodermic
{

    HYPODERMIC_DECLARE_EXCEPTION(ResolutionException);

} // namespace Hypodermic


#define HYPODERMIC_THROW_RESOLUTION_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic::ResolutionException, message)
