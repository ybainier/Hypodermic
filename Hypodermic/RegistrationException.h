#pragma once

#include "Hypodermic2/ExceptionBase.h"


namespace Hypodermic2
{

    HYPODERMIC_DECLARE_EXCEPTION(RegistrationException);

} // namespace Hypodermic2


#define HYPODERMIC_THROW_REGISTRATION_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic2::RegistrationException, message)
