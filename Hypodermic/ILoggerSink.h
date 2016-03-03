#pragma once

#include <string>

#include "Hypodermic/LogLevel.h"


namespace Hypodermic
{

    class ILoggerSink
    {
    public:
        virtual ~ILoggerSink() {}

        virtual void append(LogLevels::LogLevel level, const std::string& message) = 0;
    };

} // namespace Hypodermic