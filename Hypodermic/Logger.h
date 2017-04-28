#pragma once

#include <memory>
#include <string>

#include "Hypodermic/LogLevel.h"
#include "Hypodermic/NoopLoggerSink.h"


namespace Hypodermic
{

    class Logger
    {
    private:
        struct LoggerPrivateLock {};

    public:
        static Logger& instance()
        {
            static auto instance(std::make_shared< Logger >(LoggerPrivateLock()));
            return *instance;
        }

        static void configureSink(const std::shared_ptr< ILoggerSink >& sink)
        {
            instance().m_sink = sink;
        }

        static void configureLogLevel(LogLevels::LogLevel level)
        {
            instance().m_logLevel = level;
        }

        static LogLevels::LogLevel getLogLevel()
        {
            return instance().m_logLevel;
        }

        bool isConfiguredForLevel(LogLevels::LogLevel level) const
        {
            return static_cast< int >(level) >= static_cast< int >(m_logLevel);
        }

        void log(LogLevels::LogLevel level, const std::string& message) const
        {
            m_sink->append(level, message);
        }

        explicit Logger(const LoggerPrivateLock&)
            : m_sink(std::make_shared< NoopLoggerSink >())
            , m_logLevel(LogLevels::Error)
        {
        }

    private:
        std::shared_ptr< ILoggerSink > m_sink;
        LogLevels::LogLevel m_logLevel;
    };

} // namespace Hypodermic

