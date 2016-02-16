#include "atlas/primitives/OpenCylinder.hpp"

#include "atlas/math/Coordinates.hpp"

namespace atlas
{
    namespace primitives
    {
        struct OpenCylinder::OpenCylinderImpl
        {
            OpenCylinderImpl(float r, float h, int sdx, int sdy) :
                radius(r),
                height(h),
                subDivsX(sdx),
                subDivsY(sdy)
            { }

            OpenCylinderImpl(OpenCylinderImpl const& imp) = default;

            ~OpenCylinderImpl()
            { }

            OpenCylinderImpl* clone()
            {
                return new OpenCylinderImpl(*this);
            }

            void createVertices(std::vector<atlas::math::Point>& vertices)
            {
                USING_ATLAS_MATH_NS;

                float theta = 0.0f, z = height;
                float thetaDelta = glm::two_pi<float>() / subDivsX;
                float zDelta = height / subDivsY;

                for (int y = 0; y < subDivsY; ++y)
                {
                    theta = 0.0f;
                    z -= zDelta;

                    for (int x = 0; x < subDivsX; ++x)
                    {
                        theta = x * thetaDelta;
                        vertices.push_back(
                            cylindricalToCartesian(Point(radius, theta, z)));
                    }
                }
            }

            void createNormals(std::vector<atlas::math::Normal>& normals,
                std::vector<atlas::math::Point> const& vertices)
            {
                USING_ATLAS_MATH_NS;

                float zDelta = height / subDivsY;
                Point centre(0, 0, height);

                int i = 0;
                for (int y = 0; y < subDivsY; ++y)
                {
                    centre.y -= zDelta;

                    for (int x = 0; x < subDivsX; ++x)
                    {
                        normals.push_back(vertices[i] - centre);
                        i++;
                    }
                }
            }

            void createIndices(std::vector<GLuint>& indices)
            {
                std::vector<GLuint> upperLoop(subDivsX, 0);
                std::vector<GLuint> lowerLoop(subDivsX, 0);

                for (int y = 0; y < subDivsY - 1; ++y)
                {
                    for (int i = 0; i < subDivsX; ++i)
                    {
                        upperLoop[i] = (y * subDivsX) + i;
                        lowerLoop[i] = (y * subDivsX) + subDivsX + i;
                    }

                    for (int x = 0, i = 0; x < subDivsX; ++x, ++i)
                    {
                        int j = (i + 1) % subDivsX;
                        indices.push_back(upperLoop[i]);
                        indices.push_back(lowerLoop[i]);
                        indices.push_back(lowerLoop[j]);

                        indices.push_back(lowerLoop[j]);
                        indices.push_back(upperLoop[j]);
                        indices.push_back(upperLoop[i]);
                    }
                }
            }

            float radius, height;
            int subDivsX, subDivsY;
        };

        OpenCylinder::OpenCylinder(float radius, float height, int subDivsX,
            int subDivsY) :
            mImpl(new OpenCylinderImpl(radius, height, subDivsX, subDivsY))
        {
            mImpl->createVertices(mVertices);
            mImpl->createNormals(mNormals, mVertices);
            mImpl->createIndices(mIndices);
        }

        OpenCylinder::OpenCylinder(OpenCylinder const& oc) :
            mImpl(oc.mImpl->clone())
        { }

        OpenCylinder::~OpenCylinder()
        { }
    }
}