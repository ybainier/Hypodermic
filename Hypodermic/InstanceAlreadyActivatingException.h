#pragma once

#include "Hypodermic/ExceptionBase.h"


namespace Hypodermic
{

    HYPODERMIC_DECLARE_EXCEPTION(InstanceAlreadyActivatingException);

} // namespace Hypodermic


#define HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic::InstanceAlreadyActivatingException, message)
