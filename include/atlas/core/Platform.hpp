/**
 * \file Platform.hpp
 * \brief Defines macros that identify the platform that the code is being
 * built with. These are used to define platform-specific code within Atlas.
 * 
 * \warning
 * Be <b> very </b> careful when using these macros within your code. Unless
 * you are absolutely sure of what you are writing with these macros as 
 * guards, please refrain from using them. Cross-platform coding is by no 
 * means a simple or elegant practice and should be handled with
 * the appropriate care and respect.
 */

#ifndef ATLAS_INCLUDE_ATLAS_PLATFORM_HPP
#define ATLAS_INCLUDE_ATLAS_PLATFORM_HPP

#pragma once

/**
 * \def ATLAS_PLATFORM_WINDOWS
 * This macro is enabled whenever the code is being built on a Windows
 * machine regardless of the bitness of the application. This is used
 * for code that should run on all Windows configurations.
 * 
 * \def ATLAS_PLATFORM_WIN32
 * This is defined for 32 bit applications compiled in Windows. This is 
 * used whenever 32-bit specific operations need to be performed.
 * 
 * \def ATLAS_PLATFORM_WIN64
 * This is defined for 64 bit applications compiled in Windows. This is
 * used whenever 64-bit specific operatiosn need to be performed.
 */
#if defined(_WIN32) || defined(_WIN64)
#define ATLAS_PLATFORM_WINDOWS
#if defined(_WIN32)
#define ATLAS_PLATFORM_WIN32
#elif defined(_WIN64)
#define ATLAS_PLATFORM_WIN64
#endif

/**
 * \def ATLAS_PLATFORM_LINUX
 * This macro is enabled on all Linux platforms (independent of the distro
 * being used). This is used for Linux specific code.
 */
#elif defined(__linux__)
#define ATLAS_PLATFORM_LINUX

/**
 * \def ATLAS_PLATFORM_APPLE
 * This macro is defined on all Apple platforms. This macro acts similarly
 * to the ATLAS_PLATFORM_WINDOWS macro.
 * 
 * \def ATLAS_PLATFORM_APPLE_PPC
 * Enabled whenever the Apple computer is running a PowerPC processor. This
 * is now rare, but it is left here for legacy reasons.
 * 
 * \def ATLAS_PLATFORM_APPLE_X86
 * Enabled for all Intel-based Apple platforms.
 */
#elif defined(__APPLE__)
#define ATLAS_PLATFORM_APPLE
#if !(defined(__i386) || defined(__amd64))
#define ATLAS_PLATFORM_APPLE_PPC
#else
#define ATLAS_PLATFORM_APPLE_X86
#endif

/**
 * \def ATLAS_PLATFORM_OPENBSD
 * This is enabled for the OpenBSD operating system.
 */
#elif defined(__OpenBSD__)
#define ATLAS_PLATFORM_OPENBSD
#endif

#endif
