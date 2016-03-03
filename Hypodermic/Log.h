#pragma once

#include <sstream>

#include "Hypodermic/Logger.h"
#include "Hypodermic/LogLevel.h"


#define HYPODERMIC_LOG(logLevel, message) \
    do \
    { \
        auto&& loggerInstance = ::Hypodermic::Logger::instance(); \
        if (!loggerInstance.isConfiguredForLevel(logLevel)) \
            break; \
        \
        std::stringstream stream; \
        stream << message; \
        \
        loggerInstance.log(logLevel, stream.str()); \
    } while (false)

#define HYPODERMIC_LOG_DEBUG(message)   HYPODERMIC_LOG(::Hypodermic::LogLevels::Debug, message)
#define HYPODERMIC_LOG_INFO(message)    HYPODERMIC_LOG(::Hypodermic::LogLevels::Info, message)
#define HYPODERMIC_LOG_WARN(message)    HYPODERMIC_LOG(::Hypodermic::LogLevels::Warn, message)
#define HYPODERMIC_LOG_ERROR(message)   HYPODERMIC_LOG(::Hypodermic::LogLevels::Error, message)