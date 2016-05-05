#ifndef ATLAS_INCLUDE_ATLAS_CORE_EXCEPTION_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_EXCEPTION_HPP

#pragma once

#include "Log.hpp"

#include <stdexcept>

namespace atlas
{
    namespace core
    {
        class Exception : public std::exception
        {
        public:
            Exception(const char* msg) :
                message(msg)
            { }

            Exception(std::string const& msg) :
                message(msg)
            { }

            virtual const char* what() const throw()
            {
                std::string text = "Exception: " + message;
                CRITICAL_LOG(text);
                return text.c_str();
            }

        protected:
            std::string message;
        };

        class RuntimeException : public Exception
        {
        public:
            RuntimeException(const char* msg) :
                Exception(msg)
            { }

            RuntimeException(std::string const& msg) :
                Exception(msg)
            { }

            virtual const char* what() const throw()
            {
                std::string text = "Runtime Exception : " + message;
                CRITICAL_LOG(text);
                return text.c_str();
            }
        };

        class LogicException : public Exception
        {
            LogicException(const char* msg) :
                Exception(msg)
            { }

            LogicException(std::string const& msg) :
                Exception(msg)
            { }

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