#include "atlas/utils/Geometry.hpp"
#include "atlas/core/Macros.hpp"

namespace atlas
{
    namespace utils
    {
        Geometry::Geometry()
        { }

        Geometry::~Geometry()
        { }

        void Geometry::updateGeometry(Time const& t)
        {
            UNUSED(t);
        }

        void Geometry::renderGeometry(math::Matrix4 projection, 
            math::Matrix4 view)
        {
            UNUSED(projection);
            UNUSED(view);
        }

        void Geometry::transformGeometry(math::Matrix4 const& t)
        {
            UNUSED(t);
        }

        bool Geometry::intersectRay(math::Ray const& ray, float& tMin)
        {
            UNUSED(ray);
            UNUSED(tMin);
            return false;
        }
    }
}