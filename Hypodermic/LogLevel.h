#pragma once

#include <string>
#include <boost/lexical_cast.hpp>


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
        default:
            return "Unknown LogLevel (" + boost::lexical_cast< std::string >((int)logLevel) + ")";
        }
    }

} // namespace Hypodermic
