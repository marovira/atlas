#ifndef ATLAS_INCLUDE_ATLAS_PLATFORM_HPP
#define ATLAS_INCLUDE_ATLAS_PLATFORM_HPP

#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define ATLAS_PLATFORM_WINDOWS
#if defined(_WIN32)
#define ATLAS_PLATFORM_WIN32
#elif defined(_WIN64)
#define ATLAS_PLATFORM_WIN64
#endif

#elif defined(__linux__)
#define ATLAS_PLATFORM_LINUX

#elif defined(__APPLE__)
#define ATLAS_PLATFORM_APPLE
#if !(defined(__i386) || defined(__amd64))
#define ATLAS_PLATFORM_APPLE_PPC
#else
#define ATLAS_PLATFORM_APPLE_X86
#endif

#elif defined(__OpenBSD__)
#define ATLAS_PLATFORM_OPENBSD
#endif

#endif
