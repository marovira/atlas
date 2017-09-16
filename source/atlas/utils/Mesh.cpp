#include "atlas/utils/Mesh.hpp"
#include "atlas/core/Log.hpp"

#include <unordered_set>
#include <functional>
#include <fstream>
#include <unordered_map>
#include <numeric>

namespace atlas
{
    namespace utils
    {
        Mesh::Mesh()
        { }

        bool Mesh::fromTriangleSoup(
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
                return false;
            }

            if (hasTextures && vertices.size() != uvs.size())
            {
                WARN_LOG("Number of texture coordinates and vertices doesn't\
                         match.");
                return false;
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
                        mesh.texCoords().push_back(v.uv);
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

                    if (hasNormals)
                    {
                        normalCount[uniqueV.index]++;
                        auto k = normalCount[uniqueV.index];
                        auto mK = mesh.normals()[uniqueV.index];
                        mK = mK + ((v.normal - mK) / (float)k);
                        mesh.normals()[uniqueV.index] = mK;
                    }
                }
            }

            return true;
        }

        bool Mesh::fromFile(std::string const& filename, Mesh& mesh, 
            std::string const& mtl, bool triangulate)
        {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;

            std::string errorString;

            bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials,
                &errorString, filename.c_str(), mtl.c_str(), triangulate);

            if (!errorString.empty())
            {
                ERROR_LOG(errorString);
            }

            if (!ret)
            {
                return false;
            }

            using std::size_t;

            std::vector<math::Point> vertices;
            std::vector<math::Normal> normals;
            std::vector<math::Point2> texCoord;
            std::vector<GLuint> indices;

            for (auto& shape : shapes)
            {
                for (auto& index : shape.mesh.indices)
                {
                    vertices.emplace_back(
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    );

                    if (!attrib.normals.empty())
                    {
                        normals.emplace_back(
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2]
                        );
                    }

                    if (!attrib.texcoords.empty())
                    {
                        texCoord.emplace_back(
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1]
                        );
                    }

                }
            }

            indices.resize(vertices.size());
            std::iota(indices.begin(), indices.end(), 0);

            return fromTriangleSoup(vertices, indices, mesh, normals, texCoord);


            //size_t totalDataCount = 0;
            //for (size_t s = 0; s < shapes.size(); ++s)
            //{
            //    size_t indexOffset = 0;
            //    size_t numVertices = 0, numNormals = 0, numTex = 0,
            //        numIndices = 0;

            //    std::vector<GLuint> tmpIndices;
            //    std::vector<Face> faces;
            //    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size();
            //        ++f)
            //    {
            //        size_t fv = shapes[s].mesh.num_face_vertices[f];


            //        Face face{ indices.size() + tmpIndices.size(),
            //        fv, shapes[s].mesh.material_ids[f] };
            //        face.materialId = (face.materialId == -1) ? 
            //            0 : face.materialId;
            //        faces.push_back(face);

            //        for (size_t v = 0; v < fv; ++v)
            //        {
            //            tinyobj::index_t index =
            //                shapes[s].mesh.indices[indexOffset + v];

            //            math::Point vertex;
            //            vertex.x = attrib.vertices[3 * index.vertex_index + 0];
            //            vertex.y = attrib.vertices[3 * index.vertex_index + 1];
            //            vertex.z = attrib.vertices[3 * index.vertex_index + 2];

            //            vertices.push_back(vertex);
            //            numVertices += 3;

            //            if (!attrib.normals.empty())
            //            {
            //                math::Normal nml;
            //                nml.x = attrib.normals[3 * index.normal_index + 0];
            //                nml.y = attrib.normals[3 * index.normal_index + 1];
            //                nml.z = attrib.normals[3 * index.normal_index + 2];
            //                normals.push_back(nml);
            //                numNormals += 3;
            //            }

            //            if (!attrib.texcoords.empty())
            //            {
            //                math::Point2 t;
            //                t.x = attrib.texcoords[2 * index.texcoord_index + 0];
            //                t.y = attrib.texcoords[2 * index.texcoord_index + 1];
            //                texCoords.push_back(t);
            //                numTex += 2;
            //            }

            //            tmpIndices.push_back((GLuint)tmpIndices.size());
            //            numIndices++;
            //        }

            //        indexOffset += fv;
            //    }

            //    std::vector<Face> uniqueFaces;
            //    Face firstFace = faces[0];
            //    for (size_t i = 1; i < faces.size(); ++i)
            //    {
            //        if (faces[i].materialId == firstFace.materialId)
            //        {
            //            firstFace.size += faces[i].size;
            //        }
            //        else
            //        {
            //            uniqueFaces.push_back(firstFace);
            //            firstFace = faces[i];
            //        }

            //    }

            //    uniqueFaces.push_back(firstFace);

            //    Shape shape{ totalDataCount, uniqueFaces };
            //    mesh.shapes().push_back(shape);
            //    mesh.indices().insert(mesh.indices().end(),
            //        tmpIndices.begin(), tmpIndices.end());
            //    totalDataCount += numVertices + numNormals + numTex;
            //}

            //mesh.vertices() = vertices;
            //mesh.normals() = normals;
            //mesh.texCoords() = texCoords;
        }

        std::vector<atlas::math::Point>& Mesh::vertices()
        {
            return mVertices;
        }

        std::vector<atlas::math::Normal>& Mesh::normals()
        {
            return mNormals;
        }

        std::vector<atlas::math::Point2>& Mesh::texCoords()
        {
            return mTexCoords;
        }

        std::vector<GLuint>& Mesh::indices()
        {
            return mIndices;
        }

        std::vector<Shape>& Mesh::shapes()
        {
            return mShapes;
        }

        std::vector<tinyobj::material_t>& Mesh::materials()
        {
            return mMaterials;
        }

        void Mesh::saveToFile(std::string const& filename)
        {
            std::fstream file(filename, std::fstream::out);

            file << "# number of vertices: " << mVertices.size() << "\n";

            for (std::size_t i = 0; i < mVertices.size(); ++i)
            {
                auto v = mVertices[i];
                file << "v " << v.x << " " << v.y << " " << v.z << "\n";

                if (!mNormals.empty())
                {
                    auto n = mNormals[i];
                    file << "vn " << n.x << " " << n.y << " " << n.z << "\n";
                }

                if (!mTexCoords.empty())
                {
                    auto uv = mTexCoords[i];
                    file << "vt " << uv.x << " " << uv.y << "\n";
                }
            }

            for (std::size_t i = 0; i < mIndices.size(); ++i)
            {
                std::string start = "f ";
                auto idx = mIndices[i];

                if (mTexCoords.empty() && mNormals.empty())
                {
                    // We only have vertices.
                    file << start << idx << "\n";
                }
                else if (!mTexCoords.empty() && mNormals.empty())
                {
                    // We have textures but not normals.
                    file << start << idx << "/" << idx << "\n";
                }
                else if (mTexCoords.empty() && !mNormals.empty())
                {
                    // We have normals but not textures.
                    file << start << idx << "//" << idx << "\n";
                }
                else
                {
                    // We have normals and textures.
                    file << start << idx << "/" << idx << "/" << idx << "\n";
                }
            }

            file.close();
        }
    }
}