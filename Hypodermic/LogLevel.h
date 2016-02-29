#pragma once

#include <string>


namespace Hypodermic
{
    namespace LogLevels
    {

        enum LogLevel
        {
            Debug,
            Info,
            Warn,
            Error,
            Off,
        };

    } // namespace LogLevels


    inline std::string toString(LogLevels::LogLevel logLevel)
    {
        switch (logLevel)
        {
        case LogLevels::Debug:
            return "Debug";
        case LogLevels::Info:
            return "Info";
        case LogLevels::Warn:
            return "Warn";
        case LogLevels::Error:
            return "Error";
        case LogLevels::Off:
            return "Off";
        default:
            return "Unknown LogLevel (" + std::to_string((int)logLevel) + ")";
        }
    }

} // namespace Hypodermic
