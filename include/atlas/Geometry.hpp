#ifndef ATLAS_INCLUDE_ATLAS_GEOMETRY_HPP
#define ATLAS_INCLUDE_ATLAS_GEOMETRY_HPP

#pragma once

#include "Vector.hpp"
#include "ShadeRec.hpp"
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

        // Ray tracing functions.
        virtual bool hit(Ray const& ray, float& tMin, ShadeRec& sr);
        virtual bool shadowHit(Ray const& ray, float& tMin);
        virtual RGBColour shade(ShadeRec& sr);

        virtual void updateGeometry(Time const& t);
        virtual void renderGeometry(Matrix4 projection, Matrix4 view);
        virtual void transformGeometry(Matrix4 const& t);

        virtual Normal getNormal() const;
        virtual Normal getNormal(Point const& p) const;

    protected:
        virtual bool intersectRay(Ray const& ray, float& tMin);

        std::vector<GLShader> mShaders;
        std::map<std::string, GLuint> mUniforms;
        Matrix4 mModel;
    };
}

#endif