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
    /**
    * \class Log
    * \brief Provides a simple, cross-platform way of producing output for
    * logging and debugging purposes.
    * 
    * The output is directed to stdout (in the case of Linux and OSX) and to 
    * both stdout and the debug console for Windows (provided the console 
    * exists). The log output is formatted as follows:
    * \verbatim
    12:12:12    [flag] : <message> \endverbatim
    * The timestamp is generated using the STD <tt> \<chrono\> </tt> library.
    * The flag can be one of the following:
    * \li debug
    * \li info
    * \li log
    * \li error
    * \li critical
    *
    * These are chosen depending on the macro that is used. While it is possible
    * to invoke the log function directly, it is more convenient to use the
    * provided macros.
    * 
    
    */
        class Log
        {
        public:
            /**
             * \enum SeverityLevel
             * Provides the different types of flags that the logger supports.
             * \var DEBUG
             * Outputs the "debug" flag in the log message. The corresponding macro is
             * only enabled in debug builds.
             * \var INFO
             * Generates the "info" flag in the log message.
             * \var WARNING
             * Generated the "warning" flag in the log message. This is used
             * in Atlas whenever an operation that is invalid (but not critical)
             * happens.
             * \var ERR
             * Generates the "error" flag in the log message. Used in Atlas to denote
             * non-critical errors that the system can still recover from.
             * \var CRITICAL
             * Generates the "critical" flag in the log message. This is used in Atlas
             * for critical errors that usuallly result in the program exiting
             * immediately.
             */
            enum class SeverityLevel : int
            {
                DEBUG,
                INFO,
                WARNING,
                ERR,
                CRITICAL
            };

            /**
             * Outputs the passed in message in the format described above
             * using the given flag to stdout and the debug console (Windows 
             * only).
             * 
             * @param[in] level The severity flag for the output message.
             * @param[in] message The message to output.
             */
            static void log(SeverityLevel level, std::string const& message);
        };
    }
}

/**
 * \def LOG(level, message)
 * A shortcut to using the log function without having to type in
 * the full namespace path. This is used to construct the other macros.
 */
#define LOG(level, message) \
        atlas::core::Log::log(level, message)

/**
 * \def DEBUG_LOG(message)
 * Outputs the given message with the "debug" flag enabled.
 * \note
 * This macro performs the specified operation in debug mode only. In
 * release mode, this evaluates to nothing.
 */
#ifdef ATLAS_DEBUG
#define DEBUG_LOG(message) \
        LOG(atlas::core::Log::SeverityLevel::DEBUG, message)
#else
#define DEBUG_LOG(message)
#endif

/**
 * \def INFO_LOG(message)
 * Outputs the given message with the "info" flag enabled.
 */
#define INFO_LOG(message) \
        LOG(atlas::core::Log::SeverityLevel::INFO, message)

/**
 * \def WARN_LOG(message)
 * Outputs the given message with the "warning" flag enabled.
 */
#define WARN_LOG(message) \
        LOG(atlas::core::Log::SeverityLevel::WARNING, message)

/**
 * \def ERROR_LOG(message)
 * Outputs the given message with the "error" flag enabled.
 */
#define ERROR_LOG(message) \
        LOG(atlas::core::Log::SeverityLevel::ERR, message)

/**
 * \def CRITICAL_LOG(message)
 * Outputs the given message with the "critical" flag enabled.
 */
#define CRITICAL_LOG(message) \
         LOG(atlas::core::Log::SeverityLevel::CRITICAL, message)

#endif