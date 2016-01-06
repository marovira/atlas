#ifndef ATLAS_INCLUDE_ATLAS_CORE_LOG_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_LOG_HPP

#pragma once

#include "Core.hpp"
#include "Macros.hpp"

#include <string>

namespace atlas
{
    namespace core
    {
        class Logger
        {
        public:
            enum class SeverityLevel : int
            {
                DEBUG,
                INFO,
                WARNING,
                ERR,
                CRITICAL
            };

            static void log(SeverityLevel level, std::string const& message);
        };
    }
}

#define LOG(level, message) atlas::core::Logger::log(level, message)

#ifdef APOLLO_DEBUG
#define DEBUG_LOG(message) LOG(atlas::core::Logger::SeverityLevel::DEBUG, \
        message)
#else
#define DEBUG_LOG(message)
#endif

#define INFO_LOG(message) LOG(atlas::core::Logger::SeverityLevel::INFO, \
        message)
#define WARN_LOG(message) LOG(atlas::core::Logger::SeverityLevel::WARNING, \
        message)
#define ERROR_LOG(message) LOG(atlas::core::Logger::SeverityLevel::ERR, \
        message)
#define CRITICAL_LOG(message) LOG(\
        atlas::core::Logger::SeverityLevel::CRITICAL, message)

#endif