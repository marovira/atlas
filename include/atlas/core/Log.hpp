/**
 *	\file Log.hpp
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

#ifndef ATLAS_INCLUDE_ATLAS_CORE_LOG_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_LOG_HPP

#pragma once

#include "Macros.hpp"

#include <string>

namespace atlas
{
    namespace core
    {
        namespace Log
        {
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
             * for critical errors that usually result in the program exiting
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
            void log(SeverityLevel level, std::string const& message);

            /**
             *	Outputs the passed message in the format described above
             *	using the give flag to stdout and the debug console (Windows
             *	only). This function allows the user to pass in formatted
             *	strings like \c printf.
             *	
             *	\param[in] level The severity flag for the output message.
             *	\param[in] format The formatted string to output.
             *	\param[in] ... The format specification.
             */
            void log(SeverityLevel level, const char* format, ...);
        }
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
 *	\def LOG_V(level, format, ...)
 *	A shortcut to using the variadic log function without having to type in the 
 *	full namespace path. This is used to construct the other macros. All
 *	macros that employ the variadic log function are appended with \c _V.
 */
#define LOG_V(level, format, ...) \
        atlas::core::Log::log(level, format, __VA_ARGS__)

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

/**
 *	\def DEBUG_LOG_V(format, ...)
 *	The variadic version of \c DEBUG_LOG. Outputs the given formatted string
 *	with the "debug" flag enabled.
 *	\note
 *	This macro performs the specified operation in debug mode only. In 
 *	release mode, this evaluates to nothing.
 */
#define DEBUG_LOG_V(format, ...) \
        LOG_V(atlas::core::Log::SeverityLevel::DEBUG, format, __VA_ARGS__)
#else
#define DEBUG_LOG(message)

#define DEBUG_LOG_V(format, ...)
#endif

/**
 * \def INFO_LOG(message)
 * Outputs the given message with the "info" flag enabled.
 */
#define INFO_LOG(message) \
        LOG(atlas::core::Log::SeverityLevel::INFO, message)

/**
 *	\def INFO_LOG_V(format, ...)
 *	Variadic version of \c INFO_LOG. Outputs the given message with the 
 *	"info" flag enabled.
 */
#define INFO_LOG_V(format, ...) \
        LOG_V(atlas::core::Log::SeverityLevel::INFO, format, __VA_ARGS__)

/**
 * \def WARN_LOG(message)
 * Outputs the given message with the "warning" flag enabled.
 */
#define WARN_LOG(message) \
        LOG(atlas::core::Log::SeverityLevel::WARNING, message)

/**
 *	\def WARN_LOG_V(format, ...)
 *	Variadic version of \c WANR_LOG. Outputs the given message with the
 *	"warning" flag enabled.
 */
#define WARN_LOG_V(format, ...) \
        LOG_V(atlas::core::Log::SeverityLevel::WARNING, format, __VA_ARGS__)

/**
 * \def ERROR_LOG(message)
 * Outputs the given message with the "error" flag enabled.
 */
#define ERROR_LOG(message) \
        LOG(atlas::core::Log::SeverityLevel::ERR, message)

/**
 *	\def ERROR_LOG_V(format, ...)
 *	Variadic version of \c ERROR_LOG. Outputs the given message with the
 *	"error" flag enabled.
 */
#define ERROR_LOG_V(format, ...) \
        LOG_V(atlas::core::Log::SeverityLevel::ERR, format, __VA_ARGS__)

/**
 * \def CRITICAL_LOG(message)
 * Outputs the given message with the "critical" flag enabled.
 */
#define CRITICAL_LOG(message) \
         LOG(atlas::core::Log::SeverityLevel::CRITICAL, message)

/**
 *	\def CRITICAL_LOG_V(format, ...)
 *	Variadic version of \c CRITICAL_LOG. Outputs the given message with the
 *	"critical flag enabled".
 */
#define CRITICAL_LOG_V(format, ...) \
        LOG_V(atlas::core::Log::SeverityLevel::CRITICAL, format, __VA_ARGS__)

#endif
