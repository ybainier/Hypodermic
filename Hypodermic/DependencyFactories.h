#pragma once

#include <unordered_map>

#include "Hypodermic/DependencyFactory.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    typedef std::unordered_map< TypeInfo, DependencyFactory > DependencyFactories;

} // namespace Hypodermic