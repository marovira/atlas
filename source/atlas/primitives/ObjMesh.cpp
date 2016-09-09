#include "atlas/primitives/ObjMesh.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/TinyObjLoader.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/Buffer.hpp"

#include <unordered_map>
#include <set>
#include <tuple>


namespace atlas
{
    namespace primitives
    {

        struct Face
        {
            Face() :
                start(0),
                size(0),
                materialId(0)
            { }

            Face(size_t st, size_t sz, int mi) :
                start(st),
                size(sz),
                materialId(mi)
            { }

            size_t start;
            size_t size;
            int materialId;
        };

        struct ShapeData
        {
            ShapeData() :
                dataStart(0)
            { }

            ShapeData(size_t start, std::vector<Face> const& f) :
                dataStart(start),
                faces(f)
            { }

            size_t dataStart;
            std::vector<Face> faces;
        };

        struct MeshData
        {
            MeshData()
            { }

            std::vector<ShapeData> shapes;
            std::vector<tinyobj::material_t> materials;
            std::vector<float> rawData;
            bool hasNormals;
            bool hasTexCoords;
        };

        struct ObjMesh::ObjMeshImpl
        {
            ObjMeshImpl() :
                dataBuffer(GL_ARRAY_BUFFER),
                indexBuffer(GL_ELEMENT_ARRAY_BUFFER),
                materialBuffer(GL_UNIFORM_BUFFER)
            {
                
                materialBuffer.bindBuffer();
                materialBuffer.bufferData(
                    SIZE(5, atlas::math::Vector4) + SIZE(3, float) + 
                    SIZE(1, int),
                    nullptr, GL_STATIC_DRAW);
                materialBuffer.unBindBuffer();
            }

            ObjMeshImpl(ObjMeshImpl const& impl) = default;

            ObjMeshImpl* clone()
            {
                return new ObjMeshImpl(*this);
            }

            gl::VertexArrayObject vao;
            gl::Buffer dataBuffer;
            gl::Buffer indexBuffer;
            gl::Buffer materialBuffer;

            MeshData meshData;
            GLuint stride;
        };

        ObjMesh::ObjMesh() :
            mImpl(new ObjMeshImpl)
        { }

        ObjMesh::ObjMesh(ObjMesh const& m) :
            mImpl(m.mImpl->clone())
        { }

        ObjMesh::~ObjMesh()
        { }

        bool ObjMesh::loadFromFile(std::string const& file, 
            std::string const& mtl, bool triangulate)
        {
            USING_ATLAS_MATH_NS;

            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;

            std::string errorString;
            bool ret = tinyobj::LoadObj(&attrib, &shapes, 
                &mImpl->meshData.materials, &errorString, file.c_str(), 
                mtl.c_str(), triangulate);

            if (!errorString.empty())
            {
                ERROR_LOG(errorString);
            }

            if (!ret)
            {
                return false;
            }


            std::vector<GLuint> indices;
            std::vector<Point> vertices;
            std::vector<Normal> normals;
            std::vector<Point2> texCoords;

            size_t totalDataCount = 0;
            for (size_t s = 0; s < shapes.size(); ++s)
            {
                size_t indexOffset = 0;

                size_t numVertices = 0, numNormals = 0, numTex = 0,
                    numIndices = 0;

                std::vector<GLuint> tmpIndices;
                std::vector<Face> faces;

                for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size();
                    ++f)
                {
                    size_t fv = shapes[s].mesh.num_face_vertices[f];

                    Face face = { indices.size() + tmpIndices.size(),
                        fv, shapes[s].mesh.material_ids[f] };
                    face.materialId = (face.materialId == -1) ? 
                        0 : face.materialId;
                    faces.push_back(face);

                    for (size_t v = 0; v < fv; ++v)
                    {
                        tinyobj::index_t index =
                            shapes[s].mesh.indices[indexOffset + v];;

                        Point vertex;
                        vertex.x = attrib.vertices[3 * index.vertex_index + 0];
                        vertex.y = attrib.vertices[3 * index.vertex_index + 1];
                        vertex.z = attrib.vertices[3 * index.vertex_index + 2];

                        vertices.push_back(vertex);
                        numVertices += 3;

                        if (!attrib.normals.empty())
                        {
                            Normal normal;
                            normal.x = attrib.normals[3 * index.normal_index + 0];
                            normal.y = attrib.normals[3 * index.normal_index + 1];
                            normal.z = attrib.normals[3 * index.normal_index + 2];
                            normals.push_back(normal);
                            numNormals += 3;
                        }

                        if (!attrib.texcoords.empty())
                        {
                            Point2 tex;
                            tex.x = attrib.texcoords[2 * index.texcoord_index + 0];
                            tex.y = attrib.texcoords[2 * index.texcoord_index + 1];
                            texCoords.push_back(tex);
                            numTex += 2;
                        }

                        tmpIndices.push_back(tmpIndices.size());
                        numIndices++;
                    }
                    indexOffset += fv;
                }

                std::vector<Face> uniqueFaces;

                Face firstFace = faces[0];
                for (size_t i = 1; i < faces.size(); ++i)
                {
                    if (faces[i].materialId == firstFace.materialId)
                    {
                        firstFace.size += faces[i].size;
                    }
                    else
                    {
                        uniqueFaces.push_back(firstFace);
                        firstFace = faces[i];
                    }
                }
                uniqueFaces.push_back(firstFace);

                ShapeData shape = { totalDataCount, uniqueFaces };
                mImpl->meshData.shapes.push_back(shape);

                indices.insert(std::end(indices), std::begin(tmpIndices),
                    std::end(tmpIndices));
                totalDataCount += numVertices + numNormals + numTex;
            }

            mImpl->meshData.hasNormals = !normals.empty();
            mImpl->meshData.hasTexCoords = !texCoords.empty();

            for (size_t i = 0; i < vertices.size(); ++i)
            {
                mImpl->meshData.rawData.push_back(vertices[i].x);
                mImpl->meshData.rawData.push_back(vertices[i].y);
                mImpl->meshData.rawData.push_back(vertices[i].z);

                if (!normals.empty())
                {
                    mImpl->meshData.rawData.push_back(normals[i].x);
                    mImpl->meshData.rawData.push_back(normals[i].y);
                    mImpl->meshData.rawData.push_back(normals[i].z);
                }

                if (!texCoords.empty())
                {
                    mImpl->meshData.rawData.push_back(texCoords[i].x);
                    mImpl->meshData.rawData.push_back(texCoords[i].y);

                }
            }

            mImpl->stride = 3;
            mImpl->stride += (!normals.empty()) ? 3 : 0;
            mImpl->stride += (!texCoords.empty()) ? 2 : 0;

            mImpl->vao.bindVertexArray();
            mImpl->dataBuffer.bindBuffer();
            mImpl->dataBuffer.bufferData(
                mImpl->meshData.rawData.size() * sizeof(float),
                mImpl->meshData.rawData.data(), GL_DYNAMIC_DRAW);

            mImpl->indexBuffer.bindBuffer();
            mImpl->indexBuffer.bufferData(indices.size() * sizeof(GLuint),
                indices.data(), GL_DYNAMIC_DRAW);

            mImpl->vao.enableVertexAttribArray(0);

            if (!normals.empty())
            {
                mImpl->vao.enableVertexAttribArray(1);
            }

            if (!texCoords.empty())
            {
                mImpl->vao.enableVertexAttribArray(2);
            }

            mImpl->vao.unBindVertexArray();

            return true;
        }

        void ObjMesh::setMaterialBufferLocation(GLuint index)
        {
            mImpl->materialBuffer.bindBufferRange(index, 0,
                STRIDE(18, float) + STRIDE(1, int));
        }
        
        void ObjMesh::drawMesh()
        {
            USING_ATLAS_MATH_NS;

            mImpl->vao.bindVertexArray();
            mImpl->dataBuffer.bindBuffer();
            mImpl->materialBuffer.bindBuffer();

            GLuint dataOffset = 0;
            GLuint indexOffset = 0;

            for (auto shape : mImpl->meshData.shapes)
            {
                mImpl->dataBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                    STRIDE(mImpl->stride, float),
                    BUFFER_OFFSET(shape.dataStart, float));

                if (mImpl->meshData.hasNormals)
                {
                    mImpl->dataBuffer.vertexAttribPointer(1, 3, GL_FLOAT,
                        GL_FALSE, STRIDE(mImpl->stride, float),
                        BUFFER_OFFSET((shape.dataStart + 3), float));
                }

                if (mImpl->meshData.hasTexCoords)
                {
                    GLuint offset = (mImpl->meshData.hasNormals) ? 6 : 3;
                    mImpl->dataBuffer.vertexAttribPointer(2, 2, GL_FLOAT,
                        GL_FALSE, STRIDE(mImpl->stride, float),
                        BUFFER_OFFSET((shape.dataStart + offset), float));
                }

                int currentId = -1;
                for (auto face : shape.faces)
                {
                    if (face.materialId != currentId)
                    {
                        currentId = face.materialId;
                        tinyobj::material_t material =
                            mImpl->meshData.materials[face.materialId];

                        Vector4 mat;
                        mat = Vector4(material.ambient[0], material.ambient[1],
                            material.ambient[2], 0.0f);
                        mImpl->materialBuffer.bufferSubData(STRIDE(0, Vector4),
                            SIZE(1, Vector4), &mat[0]);

                        mat = Vector4(material.diffuse[0], material.diffuse[1],
                            material.diffuse[2], 0.0f);
                        mImpl->materialBuffer.bufferSubData(STRIDE(1, Vector4),
                            SIZE(1, Vector4), &mat[0]);

                        mat = Vector4(material.specular[0], material.specular[1],
                            material.specular[2], 0.0f);
                        mImpl->materialBuffer.bufferSubData(STRIDE(2, Vector4),
                            SIZE(1, Vector4), &mat[0]);

                        mat = Vector4(material.transmittance[0], 
                            material.transmittance[1], 
                            material.transmittance[2], 0.0f);
                        mImpl->materialBuffer.bufferSubData(STRIDE(3, Vector4),
                            SIZE(1, Vector4), &mat[0]);

                        mat = Vector4(material.emission[0], material.emission[1],
                            material.emission[2], 0.0f);
                        mImpl->materialBuffer.bufferSubData(STRIDE(4, Vector4),
                            SIZE(1, Vector4), &mat[0]);

                        mImpl->materialBuffer.bufferSubData(STRIDE(5, Vector4),
                            SIZE(1, float), &material.shininess);
                        mImpl->materialBuffer.bufferSubData(STRIDE(5, Vector4) +
                            STRIDE(1, float),
                            SIZE(1, float), &material.ior);
                        mImpl->materialBuffer.bufferSubData(STRIDE(5, Vector4) +
                            STRIDE(2, float), 
                            SIZE(1, float), &material.dissolve);
                        mImpl->materialBuffer.bufferSubData(STRIDE(5, Vector4) +
                            STRIDE(3, float), SIZE(1, int), &material.illum);
                    }

                    glDrawElements(GL_TRIANGLES, face.size, GL_UNSIGNED_INT,
                        BUFFER_OFFSET(face.start, GLuint));
                }

            }

            mImpl->materialBuffer.unBindBuffer();
            mImpl->dataBuffer.unBindBuffer();
            mImpl->vao.unBindVertexArray();
        }
    }
}