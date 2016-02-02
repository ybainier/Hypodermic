#pragma once

#include "Hypodermic/ExceptionBase.h"


namespace Hypodermic
{

    HYPODERMIC_DECLARE_EXCEPTION(RegistrationException);

} // namespace Hypodermic


#define HYPODERMIC_THROW_REGISTRATION_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic::RegistrationException, message)
