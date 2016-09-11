#include "atlas/primitives/QuadMesh.hpp"

#include <vector>

namespace atlas
{
    namespace primitives
    {
        struct QuadMesh::QuadMeshImpl
        {
            QuadMeshImpl(int sdx, int sdy, float cw, bool t) :
                subDivsX(sdx),
                subDivsY(sdy),
                cellWidth(cw),
                tessellate(t)
            { }

            QuadMeshImpl(QuadMeshImpl const& imp) = default;

            ~QuadMeshImpl()
            { }

            QuadMeshImpl* clone()
            {
                return new QuadMeshImpl(*this);
            }

            void createVertices(std::vector<atlas::math::Point>& vertices)
            {

                float totalWidth = cellWidth * subDivsX;
                float totalHeight = cellWidth * subDivsY;

                float minW = -totalWidth / 2.0f;
                float maxH = totalHeight / 2.0f;

                for (int i = 0; i < subDivsY; ++i)
                {
                    for (int j = 0; j < subDivsX; ++j)
                    {
                        vertices.push_back(math::Vector(minW + cellWidth * j,
                            maxH - cellWidth * i, 0.0f));
                    }
                }
            }

            void createIndices(std::vector<GLuint>& indices)
            {
                std::vector<GLuint> upperLoop(subDivsX, 0);
                std::vector<GLuint> lowerLoop(subDivsY, 0);

                for (int y = 0; y < subDivsY - 1; ++y)
                {
                    for (int i = 0; i < subDivsX; ++i)
                    {
                        upperLoop[i] = (y * subDivsX) + i;
                        lowerLoop[i] = (y * subDivsX) + subDivsX + i;
                    }

                    for (int i = 0; i < subDivsX - 1; ++i)
                    {
                        indices.push_back(upperLoop[i]);
                        indices.push_back(lowerLoop[i]);
                        indices.push_back(lowerLoop[i + 1]);

                        indices.push_back(lowerLoop[i + 1]);
                        indices.push_back(upperLoop[i + 1]);
                        indices.push_back(upperLoop[i]);

                    }
                }

            }

            int subDivsX, subDivsY;
            float cellWidth;
            bool tessellate;
        };

        QuadMesh::QuadMesh(bool tessellationOn) :
            mImpl(new QuadMeshImpl(10, 10, 1.0f, tessellationOn))
        {
            mImpl->createVertices(mVertices);

            mNormals = mVertices;

            mImpl->createIndices(mIndices);
        }

        QuadMesh::QuadMesh(int width, int height, float cellWidth,
            bool tessellationOn) :
            mImpl(new QuadMeshImpl(width, height, cellWidth, tessellationOn))
        {
            mImpl->createVertices(mVertices);

            mNormals = mVertices;

            mImpl->createIndices(mIndices);
        }

        QuadMesh::QuadMesh(QuadMesh const& m) :
            mImpl(m.mImpl->clone())
        { }

        QuadMesh::~QuadMesh()
        { }

        void QuadMesh::drawPrimitive(GLenum mode)
        {
            mVao.bindVertexArray();

            if (mImpl->tessellate)
            {
                glPatchParameteri(GL_PATCH_VERTICES, 3);
            }

            glDrawElements(mode, (int)mIndices.size(), GL_UNSIGNED_INT,
                (void*)0);
            mVao.unBindVertexArray();
        }
    }
}