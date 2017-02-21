#include "atlas/utils/Mesh.hpp"
#include "atlas/gl/Buffer.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/core/Log.hpp"

#include <unordered_set>
#include <functional>

namespace atlas
{
    namespace utils
    {
        struct Mesh::MeshImpl
        {
            MeshImpl() :
                vertexBuffer(GL_ARRAY_BUFFER),
                normalBuffer(GL_ARRAY_BUFFER),
                textureBuffer(GL_ARRAY_BUFFER),
                indexBuffer(GL_ELEMENT_ARRAY_BUFFER),
                isValid(false)
            { }

            MeshImpl(MeshImpl&& impl) = default;
            ~MeshImpl() = default;

            MeshImpl& operator=(MeshImpl&& rhs) = default;

            gl::Buffer vertexBuffer;
            gl::Buffer normalBuffer;
            gl::Buffer textureBuffer;
            gl::Buffer indexBuffer;
            gl::VertexArrayObject vao;
            
            GLuint numIndices;
            bool isValid;
            bool hasNormals;
            bool hasTextures;
        };

        Mesh::Mesh() :
            mImpl(std::make_unique<MeshImpl>())
        { }

        Mesh::Mesh(Mesh&& rhs) :
            mImpl(std::make_unique<MeshImpl>(std::move(*rhs.mImpl)))
        { }

        Mesh& Mesh::operator=(Mesh&& rhs)
        {
            *mImpl = std::move(*rhs.mImpl);
            return *this;
        }

        Mesh::~Mesh()
        { }

        void Mesh::fromTriangleSoup(
            std::vector<atlas::math::Point> const& vertices,
            std::vector<GLuint> const& indices,
            Mesh& mesh,
            std::vector<atlas::math::Normal> const& normals,
            std::vector<atlas::math::Point2> const& uvs)
        {
            struct MeshVertex
            {
                bool operator==(MeshVertex const& rhs) const
                {
                    return
                        (vertex == rhs.vertex) &&
                        (normal == rhs.normal) &&
                        (uv == rhs.uv);
                }

                bool operator!=(MeshVertex const& rhs) const
                {
                    return !(*this == rhs);
                }

                math::Point vertex;
                math::Normal normal;
                math::Point2 uv;
                GLuint index;
            };

            struct MeshVertexHasher
            {
                std::size_t operator()(MeshVertex const& mv) const
                {
                    return std::hash<float>()(mv.vertex.x + mv.vertex.y + 
                        mv.vertex.z);
                }
            };

            bool hasNormals, hasTextures;

            hasNormals = !normals.empty();
            hasTextures = !uvs.empty();

            if (hasNormals && vertices.size() != normals.size())
            {
                WARN_LOG("Number of normals and vertices doesn't match.");
                return;
            }

            if (hasTextures && vertices.size() != uvs.size())
            {
                WARN_LOG("Number of texture coordinates and vertices doesn't\
                         match.");
                return;
            }

            std::unordered_set<MeshVertex, MeshVertexHasher> uniqueVertices;
            std::vector<std::size_t> normalCount;
            GLuint current = 0;
            for (auto& idx : indices)
            {
                MeshVertex v;
                v.vertex = vertices[idx];

                if (hasNormals)
                {
                    v.normal = normals[idx];
                }

                if (hasTextures)
                {
                    v.uv = uvs[idx];
                }

                // Check if this vertex already exists.
                if (uniqueVertices.find(v) == uniqueVertices.end())
                {
                    // It doesn't, so increase the count and insert.
                    mesh.indices().push_back(current);
                    mesh.vertices().push_back(v.vertex);
                    normalCount.push_back(1);

                    if (hasNormals)
                    {
                        mesh.normals().push_back(v.normal);
                    }

                    if (hasTextures)
                    {
                        mesh.uvs().push_back(v.uv);
                    }

                    v.index = current;
                    uniqueVertices.insert(v);
                    current++;
                }
                else
                {
                    // We have already seen this vertex, so grab it's index.
                    auto uniqueV = *uniqueVertices.find(v);
                    mesh.indices().push_back(uniqueV.index);

                    normalCount[uniqueV.index]++;
                    auto k = normalCount[uniqueV.index];
                    auto mK = mesh.normals()[uniqueV.index];
                    mK = mK + ((v.normal - mK) / (float)k);
                    mesh.normals()[uniqueV.index] = mK;
                }
            }

            // Tell the mesh to update itself to reflect the new data.
            mesh.updateMesh();
            mesh.updateIndices();
        }

        std::vector<atlas::math::Point>& Mesh::vertices()
        {
            return mVertices;
        }

        std::vector<atlas::math::Normal>& Mesh::normals()
        {
            return mNormals;
        }

        std::vector<atlas::math::Point2>& Mesh::uvs()
        {
            return mUvs;
        }

        std::vector<GLuint>& Mesh::indices()
        {
            return mIndices;
        }

        bool Mesh::isValid() const
        {
            return mImpl->isValid;
        }

        void Mesh::setVertexAttribArrays(std::vector<GLuint> const& arrays)
        {
            if (arrays.size() < 1)
            {
                WARN_LOG("No indices provided for vertex attribute arrays.");
                mImpl->isValid = false;
                return;
            }

            mImpl->vao.bindVertexArray();
            for (auto& idx : arrays)
            {
                mImpl->vao.enableVertexAttribArray(idx);
            }

            mImpl->vertexBuffer.bindBuffer();
            mImpl->vertexBuffer.vertexAttribPointer(arrays[0],
                3, GL_FLOAT, GL_FALSE, gl::stride<math::Point>(0),
                gl::bufferOffset<math::Point>(0));

            if (mImpl->hasNormals && arrays.size() < 2)
            {
                WARN_LOG(std::string("No index for normal vertex attribute") +
                    " array provided. Normals will not be bound.");
                return;
            }

            mImpl->normalBuffer.bindBuffer();
            mImpl->normalBuffer.vertexAttribPointer(arrays[1],
                3, GL_FLOAT, GL_FALSE, gl::stride<math::Normal>(0),
                gl::bufferOffset<math::Normal>(0));

            if (mImpl->hasTextures && arrays.size() < 3)
            {
                WARN_LOG("No index for texture coordinates vertex attribute \
                    array provided. Texture coordinates will not be bound.");
                return;
            }

            mImpl->textureBuffer.bindBuffer();
            mImpl->textureBuffer.vertexAttribPointer(arrays[2],
                2, GL_FLOAT, GL_FALSE, gl::stride<math::Point2>(0),
                gl::bufferOffset<math::Point2>(0));

            mImpl->textureBuffer.unBindBuffer();
            mImpl->vao.unBindVertexArray();
        }

        void Mesh::updateMesh()
        {
            if (mVertices.empty())
            {
                WARN_LOG("Attempting to create a mesh with no vertices.");
                mImpl->isValid = false;
                return;
            }

            // Now check if we have normals or texture coordinates.
            mImpl->hasNormals = !mNormals.empty();
            mImpl->hasTextures= !mUvs.empty();

            // Now check if all the arrays have the same size.
            if (mImpl->hasNormals && mVertices.size() != mNormals.size())
            {
                WARN_LOG("Number of normals and vertices don't match.");
                mImpl->isValid = false;
                return;
            }

            if (mImpl->hasTextures && mVertices.size() != mUvs.size())
            {
                WARN_LOG("Number of texture coordinates and vertices don't \
                        match.");
                mImpl->isValid = false;
                return;
            }

            mImpl->vertexBuffer.bindBuffer();
            mImpl->vertexBuffer.bufferData(
                gl::size<math::Point>(mVertices.size()), mVertices.data(),
                GL_DYNAMIC_DRAW);
            mImpl->vertexBuffer.unBindBuffer();

            if (mImpl->hasNormals)
            {
                mImpl->normalBuffer.bindBuffer();
                mImpl->normalBuffer.bufferData(
                    gl::size<math::Normal>(mNormals.size()),
                    mNormals.data(), GL_DYNAMIC_DRAW);
                mImpl->normalBuffer.unBindBuffer();
            }

            if (mImpl->hasTextures)
            {
                mImpl->textureBuffer.bindBuffer();
                mImpl->textureBuffer.bufferData(
                    gl::size<math::Point2>(mUvs.size()), mUvs.data(),
                    GL_DYNAMIC_DRAW);
                mImpl->textureBuffer.unMapBuffer();
            }

            // At this point everything is bound and ready to go, so the mesh
            // is valid.
            mImpl->isValid = true;
        }

        void Mesh::updateIndices()
        {
            mImpl->indexBuffer.bindBuffer();
            mImpl->indexBuffer.bufferData(gl::size<GLuint>(mIndices.size()),
                mIndices.data(), GL_DYNAMIC_DRAW);
            mImpl->indexBuffer.unBindBuffer();
            mImpl->numIndices = (GLuint)mIndices.size();
        }

        void Mesh::renderMesh()
        {
            if (!mImpl->isValid)
            {
                return;
            }

            mImpl->vao.bindVertexArray();
            mImpl->indexBuffer.bindBuffer();
            glDrawElements(GL_TRIANGLES, mImpl->numIndices,
                GL_UNSIGNED_INT, gl::bufferOffset<GLuint>(0));
            mImpl->vao.unBindVertexArray();
            mImpl->indexBuffer.unBindBuffer();
        }
    }
}