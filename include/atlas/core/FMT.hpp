#pragma once

#include "Platform.hpp"

#if defined(ATLAS_PLATFORM_WINDOWS)
#    pragma warning(push)
#    pragma warning(disable : 26451)
#    pragma warning(disable : 6387)
#    pragma warning(disable : 26498)
#    pragma warning(disable : 26495)
#endif

#include <fmt/printf.h>

#if defined(ATLAS_PLATFORM_WINDOWS)
#    pragma warning(pop)
#endif
