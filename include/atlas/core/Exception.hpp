/**
 * \file Exception.hpp
 * \brief Defines custom exceptions used in framework.
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_EXCEPTION_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_EXCEPTION_HPP

#pragma once

#include "Core.hpp"
#include "Log.hpp"

#include <stdexcept>

namespace atlas
{
    namespace core
    {
        /**
         * \class Exception
         * \brief Defines a custom exception.
         * 
         * This class extends the exception class provided by the STD library
         * and links it to the existing logging system so error messages
         * can always be displayed.
         */
        class Exception : public std::exception
        {
        public:
            /**
             * Standard constructor with char array.
             * 
             * \param[in] msg The message to be displayed when the exception
             * is triggered.
             */
            Exception(const char* msg) :
                message(msg)
            { }

            /**
             * Standard constructor with string.
             * 
             * \param[in] msg The message to be displayed when the exception
             * is triggered.
             */
            Exception(std::string const& msg) :
                message(msg)
            { }

            /**
             * Constructs the error message for the exception with the
             * following format: \verbatim Exception: <message> \endverbatim.
             * The function then outputs the message to the log using the
             * critical flag and then returns the message.
             * 
             * \return The message with the specified format.
             */
            virtual const char* what() const throw()
            {
                std::string text = "Exception: " + message;
                CRITICAL_LOG(text);
                return text.c_str();
            }

        protected:
            /**
             * \var message
             * Contains the message that is displayed whenever the exception
             * is thrown.
             */
            std::string message;
        };

        /**
         * \class RuntimeException
         * \brief Defines an exception for runtime errors.
         * 
         * Extends the base Exception class and modifies the final error
         * message that is produced. This is mostly for code readability so
         * different kinds of errors can be differentiated.
         */
        class RuntimeException : public Exception
        {
        public:
            /**
             * Standard constructor with char array.
             * 
             * \param[in] msg The message to be displayed when the exception 
             * is triggered.
             */
            RuntimeException(const char* msg) :
                Exception(msg)
            { }

            /**
             * Standard constructor with string.
             * 
             * \param[in] msg The message to be displayed when the exception 
             * is triggered.
             */
            RuntimeException(std::string const& msg) :
                Exception(msg)
            { }

            /**
             * Constructs the error message for the exception with the
             * following format: 
             * \verbatim Runtime Exception: <message> \endverbatim.
             * The function then outputs the message to the log using the
             * critical flag and then returns the message.
             * 
             * \return The message with the specified format.
             */
            virtual const char* what() const throw()
            {
                std::string text = "Runtime Exception : " + message;
                CRITICAL_LOG(text);
                return text.c_str();
            }
        };

        /**
         * \class LogicException
         * \brief Defines an exception for logic errors.
         * 
         * Extends the base Exception class and modifies the final error
         * message that is produced. This is mostly for code readability so
         * different kinds of errors can be differentiated.
         */
        class LogicException : public Exception
        {
        public:
            /**
             * Standard constructor with char array.
             * 
             * \param[in] msg The message to be displayed when the exception 
             * is triggered.
             */
            LogicException(const char* msg) :
                Exception(msg)
            { }

            /**
             * Standard constructor with string.
             * 
             * \param[in] msg The message to be displayed when the exception 
             * is triggered.
             */
            LogicException(std::string const& msg) :
                Exception(msg)
            { }

            /**
             * Constructs the error message for the exception with the
             * following format: 
             * \verbatim Logic Exception: <message> \endverbatim.
             * The function then outputs the message to the log using the
             * critical flag and then returns the message.
             * 
             * \return The message with the specified format.
             */
            virtual const char* what() const throw()
            {
                std::string text = "Logic Exception: " + message;
                CRITICAL_LOG(text);
                return text.c_str();
            }
        };

    }
}

#endif