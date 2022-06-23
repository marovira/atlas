#pragma once

#include <GL/gl3w.h>

#include <cstdint>
#include <ctime>
#include <optional>
#include <string>
#include <vector>

namespace atlas::glx
{
    struct FileData
    {
        FileData(std::string name, int p, std::time_t time) :
            filename{std::move(name)}, parent{p}, last_write{time}
        {}

        std::string filename;
        int parent;
        std::time_t last_write;
    };

    struct ShaderFile
    {
        std::string filename;
        std::string source_string;
        std::vector<FileData> included_files;
    };

    ShaderFile
    read_shader_source(std::string const& filename,
                       std::vector<std::string> const& include_dirs = {});

    bool should_shader_be_reloaded(ShaderFile const& file);

    std::optional<std::string> compile_shader(std::string const& source,
                                              GLuint handle);
    std::optional<std::string> link_shaders(GLuint handle);

    std::string parse_error_log(ShaderFile const& file, std::string const& log);

    bool reload_shader(GLuint program_handle,
                       GLuint shader_handle,
                       ShaderFile& file,
                       std::vector<std::string> const& include_dirs = {});

    std::optional<std::string> create_separable_shader_program(
        GLenum type, GLuint program, ShaderFile const& file);

    bool reload_separable_shader_program(
        GLenum type,
        GLuint program_handle,
        ShaderFile& file,
        std::vector<std::string> const& include_dirs = {});
} // namespace atlas::glx
