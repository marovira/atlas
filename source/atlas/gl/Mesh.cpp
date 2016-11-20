#include "atlas/gl/Mesh.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/gl/Buffer.hpp"
#include "atlas/gl/VertexArrayObject.hpp"

namespace atlas
{
    namespace gl
    {
        struct Mesh::MeshImpl
        {
            MeshImpl() :
                dataBuffer(GL_ARRAY_BUFFER),
                indexBuffer(GL_ELEMENT_ARRAY_BUFFER)
            { }

            MeshImpl(MeshImpl&& impl) = default;
            MeshImpl& operator=(MeshImpl&& rhs) = default;

            ~MeshImpl() = default;

            Buffer dataBuffer;
            Buffer indexBuffer;
            VertexArrayObject vao;
            GLuint numIndices;
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

        void Mesh::updateMesh(std::vector<atlas::math::Point> const& vertices,
            std::vector<atlas::math::Normal> const& normals,
            std::vector<atlas::math::Point2> const& textures,
            std::vector<GLuint> const& indices)
        {
            // First check if we have any vertices.
            if (vertices.empty())
            {
                WARN_LOG("Attempting to create a mesh with no vertices.");
                return;
            }

            mImpl->hasNormals = !normals.empty();
            mImpl->hasTextures = !textures.empty();
            mImpl->numIndices = (GLuint)indices.size();

            // Compile all the data into a single array. We interleave it as
            // VxVyVz/NxNyNz/TuTv.
            std::vector<float> data;
            for (size_t i = 0; i < vertices.size(); ++i)
            {
                data.push_back(vertices[i].x);
                data.push_back(vertices[i].y);
                data.push_back(vertices[i].z);

                // If we have normals, add them.
                if (mImpl->hasNormals)
                {
                    data.push_back(normals[i].x);
                    data.push_back(normals[i].y);
                    data.push_back(normals[i].z);
                }

                // If we have texture coordinates, add them.
                if (mImpl->hasTextures)
                {
                    data.push_back(textures[i].x);
                    data.push_back(textures[i].y);
                }
            }


            // Now allocate the memory from OpenGL and send it in.
            mImpl->vao.bindVertexArray();
            mImpl->dataBuffer.bindBuffer();

            // This may be inefficient. We are essentially acting under the
            // assumption that every time we update, the amount of data may
            // change, so we just re-allocate the buffer. Perhaps a better way
            // to handle this would be to check the size and simply replace the
            // data?
            mImpl->dataBuffer.bufferData(size<float>(data.size()),
                data.data(), GL_DYNAMIC_DRAW);

            mImpl->indexBuffer.bindBuffer();
            mImpl->indexBuffer.bufferData(size<GLuint>(indices.size()),
                indices.data(), GL_DYNAMIC_DRAW);

            mImpl->vao.unBindVertexArray();
            mImpl->dataBuffer.unBindBuffer();
            mImpl->indexBuffer.unBindBuffer();
        }

        void Mesh::setVertexAttribArrays(std::vector<GLuint> const& arrays)
        {
            mImpl->vao.bindVertexArray();
            mImpl->dataBuffer.bindBuffer();
            mImpl->indexBuffer.bindBuffer();

            // Compute the offset in the layout. At minimum, it will be 3 
            // (assuming only vertices).
            GLuint stride = 3;
            stride += (mImpl->hasNormals) ? 3 : 0;
            stride += (mImpl->hasTextures) ? 2 : 0;

            mImpl->dataBuffer.vertexAttribPointer(arrays[0],
                3, GL_FLOAT, GL_FALSE, gl::stride<float>(stride), 
                bufferOffset<float>(0));

            if (mImpl->hasNormals)
            {
                mImpl->dataBuffer.vertexAttribPointer(arrays[1],
                    3, GL_FLOAT, GL_FALSE, gl::stride<float>(stride),
                    bufferOffset<float>(3));
            }

            if (mImpl->hasTextures)
            {
                GLuint offset = (mImpl->hasNormals) ? 6 : 3;
                mImpl->dataBuffer.vertexAttribPointer(arrays[2],
                    2, GL_FLOAT, GL_FALSE, gl::stride<float>(stride),
                    bufferOffset<float>(offset));
            }

            for (auto& idx : arrays)
            {
                mImpl->vao.enableVertexAttribArray(idx);
            }

            mImpl->vao.unBindVertexArray();
            mImpl->dataBuffer.unBindBuffer();
            mImpl->indexBuffer.unBindBuffer();
        }

        void Mesh::render()
        {
            mImpl->vao.bindVertexArray();
            glDrawElements(GL_TRIANGLES, mImpl->numIndices, GL_UNSIGNED_INT, 
                bufferOffset<GLuint>(0));
        }
    }
}