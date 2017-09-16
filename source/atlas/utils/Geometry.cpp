#include "atlas/utils/Geometry.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/gl/Shader.hpp"

namespace atlas
{
    namespace utils
    {
        Geometry::Geometry()
        { }
        
        Geometry::~Geometry()
        { }

        void Geometry::updateGeometry(atlas::core::Time<> const& t)
        {
            UNUSED(t);
        }

        void Geometry::renderGeometry(math::Matrix4 const& projection, 
            math::Matrix4 const& view)
        {
            UNUSED(projection);
            UNUSED(view);
        }

        void Geometry::renderGeometry()
        { }

        void Geometry::drawGui()
        { }

        void Geometry::transformGeometry(math::Matrix4 const& t)
        {
            UNUSED(t);
        }

        void Geometry::resetGeometry()
        { }

        bool Geometry::intersectRay(math::Ray const& ray, float& tMin)
        {
            UNUSED(ray);
            UNUSED(tMin);
            return false;
        }

        void Geometry::reloadShaders()
        {
            for (auto& shader : mShaders)
            {
                shader.reloadShaders();
            }
        }
    }
}