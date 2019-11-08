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
            filename{std::move(name)},
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

    bool shouldShaderBeReloaded(ShaderFile const& file);

    std::optional<std::string> compileShader(std::string const& source,
                                             GLuint handle);
    std::optional<std::string> linkShaders(GLuint handle);

    std::string parseErrorLog(ShaderFile const& file, std::string const& log);

    bool reloadShader(GLuint programHandle,
                      GLuint shaderHandle,
                      ShaderFile& file,
                      std::vector<std::string> const& includeDirectories = {});
} // namespace atlas::glx
