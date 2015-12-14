#include "atlas/Geometry.hpp"
#include "atlas/Macros.hpp"

namespace atlas
{
    Geometry::Geometry()
    { }

    Geometry::~Geometry()
    { }

    bool Geometry::hit(Ray const& ray, float& tMin, ShadeRec& sr)
    {
        UNUSED(ray);
        UNUSED(tMin);
        UNUSED(sr);
        return false;
    }

    bool Geometry::shadowHit(Ray const& ray, float& tMin)
    {
        UNUSED(ray);
        UNUSED(tMin);
        return false;
    }

    RGBColour Geometry::shade(ShadeRec& sr)
    {
        UNUSED(sr);
        return black();
    }

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

    Normal Geometry::getNormal() const
    {
        return Normal();
    }

    Normal Geometry::getNormal(atlas::Point const& p) const
    {
        UNUSED(p);
        return Normal();
    }

    bool Geometry::intersectRay(Ray const& ray, float& tMin)
    {
        UNUSED(ray);
        UNUSED(tMin);
        return false;
    }
}