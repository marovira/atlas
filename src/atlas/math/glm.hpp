#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

#include <zeus/platform.hpp>

#if defined(ZEUS_PLATFORM_WINDOWS)
#    pragma warning(push)
#    pragma warning(disable : 4201)
#endif

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/projection.hpp>

#if defined(ZEUS_PLATFORM_WINDOWS)
#    pragma warning(pop)
#endif
