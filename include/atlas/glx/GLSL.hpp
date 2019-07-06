#pragma once

#include <GL/gl3w.h>

#include <cstdint>
#include <experimental/filesystem>
#include <optional>
#include <string>
#include <vector>

namespace atlas::glx
{
    struct FileData
    {
        FileData(std::string const& name, int p, std::time_t time) :
            filename{name},
            parent{p},
            lastWrite{time}
        {}

        std::string filename;
        int parent;
        std::time_t lastWrite;
    };

    struct ShaderFile
    {
        std::string filename;
        std::string sourceString;
        std::vector<FileData> includedFiles;
    };

    ShaderFile
    readShaderSource(std::string const& filename,
                     std::vector<std::string> const& includeDirectories = {});

    std::optional<std::string> compileShader(std::string const& source,
                                             GLuint handle);

    std::string parseErrorLog(ShaderFile const& file, std::string const& log);
} // namespace atlas::glx
