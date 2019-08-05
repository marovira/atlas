#pragma once

#include <atlas/core/Platform.hpp>

// TODO: Check if clang requires any specific flags to be set here.

#if defined(ATLAS_PLATFORM_WINDOWS)
#    if defined(ATLAS_WINDOWS_EXPORT)
#        if defined(__GNUC__)
#            define EXPORTED __attribute__((dllexport))
#        else
#            define EXPORTED __declspec(dllexport)
#        endif
#    else
#        if defined(__GNUC__)
#            define EXPORTED __attribute__((dllimport))
#        else
#            define EXPORTED __declspec(dllimport)
#        endif
#    endif
#    define NOT_EXPORTED
#else
#    if __GNUC__ >= 4
#        define EXPORTED __attribute__((visibility("default")))
#        define NOT_EXPORTED __attribute__((visibility("hidden")))
#    else
#        define EXPORTED
#        define NOT_EXPORTED
#    endif
#endif
