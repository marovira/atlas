/**
 * \file Macros.hpp
 * \brief Defines some useful macros used throughout Atlas.
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_MACROS_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_MACROS_HPP

#pragma once

/**
 * \def UNUSED(expr)
 * Used for functions that have unused parameters. This macro prevents the
 * compiler from giving the "unused parameter" warning. 
 * \warning
 * Unless the parameter is meant to be left unused (virtual functions for 
 * example), then <b> do not </b> use it just to silence a warning!
 */
#define UNUSED(expr) (void)expr

 /**
 * \def ATLAS_DEBUG
 * This macro is defined whenever the code is being built with debug
 * symbols enabled. It can be used to enable/disable debugging code as
 * required.
 */
#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG__)
#define ATLAS_DEBUG
#endif

#endif