#include "atlas/Geometry.hpp"
#include "atlas/Macros.hpp"

namespace atlas
{
    Geometry::Geometry()
    { }

    Geometry::~Geometry()
    { }

    void Geometry::updateGeometry(Time const& t)
    {
        UNUSED(t);
    }

    void Geometry::renderGeometry(Matrix4 projection, Matrix4 view)
    {
        UNUSED(projection);
        UNUSED(view);
    }

    void Geometry::transformGeometry(Matrix4 const& t)
    {
        UNUSED(t);
    }

    bool Geometry::intersectRay(Ray const& ray, float& tMin)
    {
        UNUSED(ray);
        UNUSED(tMin);
        return false;
    }
}