#pragma once

#include "Hypodermic2/ExceptionBase.h"


namespace Hypodermic2
{

    HYPODERMIC_DECLARE_EXCEPTION(InstanceAlreadyActivatingException);

} // namespace Hypodermic2


#define HYPODERMIC_THROW_INSTANCE_ALREADY_ACTIVATING_EXCEPTION(message) HYPODERMIC_THROW(::Hypodermic2::InstanceAlreadyActivatingException, message)
