#include "atlas/GLCamera.hpp"
#include "atlas/Macros.hpp"

namespace atlas
{
    GLCamera::GLCamera()
    { }

    GLCamera::~GLCamera()
    { }

    void GLCamera::mouseDown(Point2 const& point, CameraMovements movement)
    {
        UNUSED(point);
        UNUSED(movement);
    }

    void GLCamera::mouseDrag(Point2 const& point)
    {
        UNUSED(point);
    }

    void GLCamera::mouseUp()
    { }

    void GLCamera::resetCamera()
    { }

    Matrix4 GLCamera::getCameraMatrix()
    {
        return Matrix4();
    }

    
}