#pragma once

#include "Hypodermic/ILoggerSink.h"


namespace Hypodermic
{

    class NoopLoggerSink : public ILoggerSink
    {
    public:
        void append(LogLevels::LogLevel, const std::string&) override
        {
        }
    };

} // namespace Hypodermic