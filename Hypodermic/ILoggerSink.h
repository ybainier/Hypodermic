#pragma once

#include <string>

#include <Hypodermic2/LogLevel.h>


namespace Hypodermic2
{

    class ILoggerSink
    {
    public:
        virtual ~ILoggerSink() {}

        virtual void append(LogLevels::LogLevel level, const std::string& message) = 0;
    };

} // namespace Hypodermic2