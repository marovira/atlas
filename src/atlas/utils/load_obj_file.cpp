#include "load_obj_file.hpp"

#include <algorithm>
#include <fmt/printf.h>
#include <functional>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <zeus/filesystem.hpp>

namespace atlas::utils
{
    bool operator==(Vertex const& rhs, Vertex const& lhs)
    {
        return rhs.position == lhs.position;
    }

    struct VertexHash
    {
        std::size_t operator()(Vertex const& vertex) const
        {
            auto posHash    = std::hash<glm::vec3>()(vertex.position);
            auto normalHash = std::hash<glm::vec3>()(vertex.normal);
            auto texHash    = std::hash<glm::vec2>()(vertex.tex_coord);
            return ((posHash ^ (normalHash << 1)) >> 1) ^ (texHash << 1);
        }
    };

    std::optional<ObjMesh> load_obj_mesh(std::string const& filename,
                                         std::string const& material_path)
    {
        tinyobj::ObjReader reader;
        tinyobj::ObjReaderConfig config;
        config.triangulate  = true;
        config.vertex_color = false;

        // Check if the material path is empty. If it is, then use the root
        // directory of the file itself. Otherwise, use the provided path.
        std::string root_dir   = zeus::get_file_directory(filename);
        config.mtl_search_path = (material_path.empty()) ? root_dir : material_path;

        bool ret = reader.ParseFromFile(filename, config);
        if (!ret || !reader.Valid())
        {
            return {};
        }

        if (!reader.Warning().empty())
        {
            fmt::print("warning: in function loadObjMesh: {}\n", reader.Warning());
        }

        if (!reader.Error().empty())
        {
            fmt::print("error: in function loadObjMesh: {}\n", reader.Error());
        }

        auto attrib    = reader.GetAttrib();
        auto shapes    = reader.GetShapes();
        auto materials = reader.GetMaterials();

        if (materials.empty())
        {
            materials.push_back(tinyobj::material_t{});
        }

        ObjMesh result_mesh;
        for (std::size_t s{0}; s < shapes.size(); ++s)
        {
            Shape shape{};
            std::unordered_map<Vertex, std::size_t, VertexHash> vertex_map{};
            std::vector<Vertex> unique_vertices{};
            std::size_t index_offset{0};

            for (std::size_t face{0}; face < shapes[s].mesh.num_face_vertices.size();
                 ++face)
            {
                int num_face_vertices = shapes[s].mesh.num_face_vertices[face];
                for (int v{0}; v < num_face_vertices; ++v)
                {
                    glm::vec3 p{};
                    glm::vec3 n{};
                    glm::vec2 tex{};

                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    p.x = attrib.vertices[3 * idx.vertex_index + 0];
                    p.y = attrib.vertices[3 * idx.vertex_index + 1];
                    p.z = attrib.vertices[3 * idx.vertex_index + 2];

                    if (idx.normal_index != -1 && shape.has_normals)
                    {
                        n.x = attrib.normals[3 * idx.normal_index + 0];
                        n.y = attrib.normals[3 * idx.normal_index + 1];
                        n.z = attrib.normals[3 * idx.normal_index + 2];
                    }
                    else
                    {
                        shape.has_normals = false;
                    }

                    if (idx.texcoord_index != -1 && shape.has_texture_coords)
                    {
                        tex.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                        tex.y = attrib.texcoords[2 * idx.texcoord_index + 1];
                    }
                    else
                    {
                        shape.has_texture_coords = false;
                    }

                    Vertex vertex{};
                    vertex.position  = p;
                    vertex.normal    = n;
                    vertex.tex_coord = tex;

                    if (vertex_map.count(vertex) == 0)
                    {
                        vertex.index       = vertex_map.size();
                        vertex.face_id     = face;
                        vertex_map[vertex] = vertex.index;
                        unique_vertices.push_back(vertex);
                    }

                    shape.indices.push_back(vertex_map[vertex]);
                }
                index_offset += num_face_vertices;
            }

            shape.vertices            = unique_vertices;
            shape.material_ids        = shapes[s].mesh.material_ids;
            shape.smoothing_group_ids = shapes[s].mesh.smoothing_group_ids;

            result_mesh.shapes.push_back(shape);
        }

        result_mesh.materials = materials;
        return {result_mesh};
    }
} // namespace atlas::utils
