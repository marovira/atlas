#include "atlas/primitives/Sphere.hpp"

#include "atlas/math/Vector.hpp"
#include "atlas/math/Coordinates.hpp"
#include <vector>

namespace atlas
{
    namespace primitives
    {
        struct Sphere::SphereImpl
        {
            SphereImpl(float r, int sd) :
                radius(r),
                subDivs(sd)
            { }

            SphereImpl(SphereImpl const& imp) = default;

            ~SphereImpl()
            { }

            SphereImpl* clone()
            {
                return new SphereImpl(*this);
            }

            void createVertices(std::vector<atlas::math::Point>& vertices)
            {
                USING_ATLAS_MATH_NS;

                float theta = 0.0f, phi = 0.0f;
                float phiDelta = glm::pi<float>() / subDivs;
                float thetaDelta = glm::two_pi<float>() / subDivs;

                vertices.push_back(Point(0, 0, radius));

                for (int y = 0; y < subDivs - 1; ++y)
                {
                    phi += phiDelta;
                    theta = 0.0f;

                    for (int x = 0; x < subDivs; ++x)
                    {
                        theta = x * thetaDelta;
                        vertices.push_back(
                            sphericalToCartesian(Point(radius, theta, phi)));
                    }
                }

                vertices.push_back(Point(0, 0, -radius));
            }

            void createIndices(std::vector<GLuint>& indices, int numVertices)
            {
                std::vector<GLuint> upperLoop(subDivs, 0);
                std::vector<GLuint> lowerLoop(subDivs, 0);

                for (int i = 0; i < subDivs; ++i)
                {
                    lowerLoop[i] = i + 1;
                }

                for (int i = 0; i < subDivs; ++i)
                {
                    indices.push_back(upperLoop[i]);
                    indices.push_back(lowerLoop[i]);
                    indices.push_back(lowerLoop[(i + 1) % subDivs]);
                }

                for (int y = 0; y < subDivs - 2; ++y)
                {
                    for (int i = 0; i < subDivs; ++i)
                    {
                        upperLoop[i] = (y * subDivs + 1) + i;
                        lowerLoop[i] = (y * subDivs + 1) + subDivs + i;
                    }

                    for (int x = 0, i = 0; x < subDivs; ++x, ++i)
                    {
                        int j = (i + 1) % subDivs;
                        indices.push_back(upperLoop[i]);
                        indices.push_back(lowerLoop[i]);
                        indices.push_back(lowerLoop[j]);

                        indices.push_back(lowerLoop[j]);
                        indices.push_back(upperLoop[j]);
                        indices.push_back(upperLoop[i]);
                    }
                }

                for (int i = 0; i < subDivs; ++i)
                {
                    lowerLoop[i] = numVertices - 1;
                    upperLoop[i] = numVertices - 1 - subDivs + i;
                }

                for (int i = 0; i < subDivs; ++i)
                {
                    indices.push_back(lowerLoop[i]);
                    indices.push_back(upperLoop[i]);
                    indices.push_back(upperLoop[(i + 1) % subDivs]);
                }
            }

            float radius;
            int subDivs;
        };

        Sphere::Sphere(float radius, int subDivs) :
            mImpl(new SphereImpl(radius, subDivs)),
            Primitive()
        {
            USING_ATLAS_MATH_NS;

            mImpl->createVertices(mVertices);

            mNormals = mVertices;

            mImpl->createIndices(mIndices, (int)mVertices.size());

        }

        Sphere::Sphere(Sphere const& s) :
            mImpl(s.mImpl->clone())
        { }

        Sphere::~Sphere()
        { }
    }
}