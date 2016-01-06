#ifndef ATLAS_INCLUDE_ATLAS_UTILS_GEOMETRY_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_GEOMETRY_HPP

#pragma once

#include "Utils.hpp"
#include "Time.hpp"
#include "atlas/math/Vector.hpp"
#include "atlas/math/Ray.hpp"
#include "atlas/math/Matrix.hpp"
#include "atlas/gl/GL.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>

namespace atlas
{
    namespace utils
    {
        class Geometry
        {
        public:
            Geometry();
            Geometry(Geometry const& geom);
            virtual ~Geometry();

            virtual void updateGeometry(Time const& t);
            virtual void renderGeometry(math::Matrix4 projection, 
                math::Matrix4 view);
            virtual void transformGeometry(math::Matrix4 const& t);

        protected:
            virtual bool intersectRay(math::Ray const& ray, float& tMin);

            typedef std::unique_ptr<gl::Shader> ShaderPointer;
            std::vector<ShaderPointer> mShaders;
            typedef std::pair<std::string, GLuint> UniformKey;
            std::map<std::string, GLuint> mUniforms;
            math::Matrix4 mModel;
        };
    }
}

#endif