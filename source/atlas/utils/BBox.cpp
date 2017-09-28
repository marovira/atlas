#include "atlas/utils/BBox.hpp"

#include "atlas/math/Ray.hpp"
#include "atlas/core/Constants.hpp"
#include "atlas/core/Float.hpp"

namespace atlas
{
    namespace utils
    {
        BBox::BBox() :
            pMin(atlas::core::infinity()),
            pMax(atlas::core::negInfinity())
        { }

        BBox::BBox(atlas::math::Point const& p) :
            pMin(p),
            pMax(p)
        { }

        BBox::BBox(atlas::math::Point const& p1, atlas::math::Point const& p2)
        {
            using atlas::math::Point;

            pMin = Point(
                glm::min(p1.x, p2.x),
                glm::min(p1.y, p2.y),
                glm::min(p1.z, p2.z));
            pMax = Point(
                glm::max(p1.x, p2.x),
                glm::max(p1.y, p2.y),
                glm::max(p1.z, p2.z));
        }

        bool BBox::overlaps(BBox const& b) const
        {
            using atlas::core::geq;
            using atlas::core::leq;

            bool x = (geq(pMax.x, b.pMin.x) && leq(pMin.x, b.pMax.x));
            bool y = (geq(pMax.y, b.pMin.y) && leq(pMin.y, b.pMax.y));
            bool z = (geq(pMax.z, b.pMin.z) && leq(pMin.z, b.pMax.z));

            return (x && y && z);
        }

        bool BBox::inside(atlas::math::Point const& p) const
        {
            using atlas::core::geq;
            using atlas::core::leq;

            return (
                geq(p.x, pMin.x) && leq(p.x, pMax.x) &&
                geq(p.y, pMin.y) && leq(p.y, pMax.y) &&
                geq(p.z, pMin.z) && leq(p.z, pMax.z));
        }

        void BBox::expand(float delta)
        {
            pMin -= delta;
            pMax += delta;
        }

        bool BBox::intersect(atlas::math::Ray const& ray, float& hit0,
            float& hit1)
        {
            using atlas::core::infinity;
            float t0 = 0.0f, t1 = infinity();
            for (int i = 0; i < 3; ++i)
            {
                float invRayDir = 1.0f / ray.d[i];
                float tNear = (pMin[i] - ray.o[i]) * invRayDir;
                float tFar = (pMax[i] - ray.o[i]) * invRayDir;

                if (tNear > tFar)
                {
                    std::swap(tNear, tFar);
                }

                t0 = (tNear > t0) ? tNear : t0;
                t1 = (tFar < t1) ? tFar : t1;

                if (t0 > t1)
                {
                    return false;
                }
            }

            hit0 = t0, hit1 = t1;
            return true;
        }

        bool BBox::intersect(atlas::math::Ray const& ray)
        {
            float t1, t2;
            return intersect(ray, t1, t2);
        }

        BBox join(BBox const& b1, BBox const& b2)
        {
            BBox ret = b1;
            ret.pMin.x = glm::min(b1.pMin.x, b2.pMin.x);
            ret.pMin.y = glm::min(b1.pMin.y, b2.pMin.y);
            ret.pMin.z = glm::min(b1.pMin.z, b2.pMin.z);

            ret.pMax.x = glm::max(b1.pMax.x, b2.pMax.x);
            ret.pMax.y = glm::max(b1.pMax.y, b2.pMax.y);
            ret.pMax.z = glm::max(b1.pMax.z, b2.pMax.z);

            return ret;
        }
    }
}