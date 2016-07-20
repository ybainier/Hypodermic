#pragma once

#include <sstream>

#include "Hypodermic/Logger.h"
#include "Hypodermic/LogLevel.h"
#include "Hypodermic/Pragmas.h"

#define HYPODERMIC_LOG(hypodermicLogLevel, hypodermicLogMessage) \
    HYPODERMIC_PRAGMA_PUSH \
    HYPODERMIC_IGNORE_CONDITIONAL_EXPRESSION_IS_CONSTANT \
    do \
    { \
        auto&& hypodermicLoggerInstance = ::Hypodermic::Logger::instance(); \
        if (!hypodermicLoggerInstance.isConfiguredForLevel(hypodermicLogLevel)) \
            break; \
        \
        std::stringstream hypodermicStream; \
        hypodermicStream << hypodermicLogMessage; \
        \
        hypodermicLoggerInstance.log(hypodermicLogLevel, hypodermicStream.str()); \
    } while (false); \
    HYPODERMIC_PRAGMA_POP

#define HYPODERMIC_LOG_DEBUG(message)   HYPODERMIC_LOG(::Hypodermic::LogLevels::Debug, message)
#define HYPODERMIC_LOG_INFO(message)    HYPODERMIC_LOG(::Hypodermic::LogLevels::Info, message)
#define HYPODERMIC_LOG_WARN(message)    HYPODERMIC_LOG(::Hypodermic::LogLevels::Warn, message)
#define HYPODERMIC_LOG_ERROR(message)   HYPODERMIC_LOG(::Hypodermic::LogLevels::Error, message)
