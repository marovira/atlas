#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL

#include <atlas/core/Platform.hpp>

#if defined(ATLAS_PLATFORM_WINDOWS)
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

namespace atlas::math
{
    using Vector2 = glm::vec2;

    using Vector = glm::vec3;

    using Vector4 = glm::vec4;

    using Point2 = Vector2;

    using Point = Vector;

    using Point4 = Vector4;

    using Normal2 = Vector2;

    using Normal = Vector;

    using Normal4 = Vector4;

    using Matrix2 = glm::mat2;

    using Matrix3 = glm::mat3;

    using Matrix4 = glm::mat4;

    using Quaternion = glm::quat;
} // namespace atlas::math

#if defined(ATLAS_PLATFORM_WINDOWS)
#    pragma warning(pop)
#endif
