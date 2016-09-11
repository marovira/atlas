/**
 * \file Core.hpp
 * \brief Main header file for core module.
 * 
 * Defines forward declarations for all classes, structs, and typedefs in the 
 * module.
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_CORE_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_CORE_HPP

#pragma once

namespace atlas
{
    namespace core
    {
        class Exception;
        class RuntimeException;
        class LogicException;

        template <class GenType = float>
        class Timer;
        template <typename GenType = float>
        class Time;
    }
}

#endif