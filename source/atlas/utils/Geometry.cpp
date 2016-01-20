#include "atlas/utils/Geometry.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/gl/Shader.hpp"

namespace atlas
{
    namespace utils
    {
        Geometry::Geometry()
        { }
        
        Geometry::Geometry(Geometry const& geom) :
            mUniforms(geom.mUniforms),
            mModel(geom.mModel)
        {
            for (auto& shader : geom.mShaders)
            {
                mShaders.emplace_back(ShaderPointer(new gl::Shader(*shader)));
            }
        }

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

        void Geometry::resetGeometry()
        { }

        bool Geometry::intersectRay(math::Ray const& ray, float& tMin)
        {
            UNUSED(ray);
            UNUSED(tMin);
            return false;
        }
    }
}