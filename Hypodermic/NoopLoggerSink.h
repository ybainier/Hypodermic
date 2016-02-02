#pragma once

#include <Hypodermic2/ILoggerSink.h>


namespace Hypodermic2
{

    class NoopLoggerSink : public ILoggerSink
    {
    public:
        void append(LogLevels::LogLevel, const std::string&) override
        {
        }
    };

} // namespace Hypodermic2