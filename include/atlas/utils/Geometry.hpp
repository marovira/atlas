#ifndef ATLAS_INCLUDE_ATLAS_GEOMETRY_HPP
#define ATLAS_INCLUDE_ATLAS_GEOMETRY_HPP

#pragma once

#include "Vector.hpp"
#include "Ray.hpp"
#include "Matrix.hpp"
#include "GLShader.hpp"
#include "Time.hpp"

#include <vector>
#include <map>
#include <string>

namespace atlas
{
    class Geometry
    {
    public:
        Geometry();
        Geometry(Geometry const& geom) = default;
        virtual ~Geometry();

        virtual void updateGeometry(Time const& t);
        virtual void renderGeometry(Matrix4 projection, Matrix4 view);
        virtual void transformGeometry(Matrix4 const& t);

    protected:
        virtual bool intersectRay(Ray const& ray, float& tMin);

        std::vector<GLShader> mShaders;
        typedef std::pair<std::string, GLuint> UniformKey;
        std::map<std::string, GLuint> mUniforms;
        Matrix4 mModel;
    };
}

#endif