#include "atlas/glx/GLSL.hpp"

#if defined(max)
#undef max
#endif

#if defined(min)
#undef min
#endif

#include <fmt/printf.h>

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>

namespace fs = std::experimental::filesystem;

namespace atlas::glx
{
    std::string readShaderSource(FileData const& data, ShaderFile& shaderFile);

    std::time_t getFileLastWrite(std::string const& filename)
    {
        fs::path filePath{filename};
        auto ftime = fs::last_write_time(filePath);
        return decltype(ftime)::clock::to_time_t(ftime);
    }

    ShaderFile loadShaderFile(std::string const& filename)
    {
        ShaderFile returnFile;
        std::hash<std::string> stringHash;
        FileData data{filename, -1, stringHash(filename), {}};
        returnFile.sourceString = readShaderSource(data, returnFile);
        return returnFile;
    }

    std::string readShaderSource(FileData const& data, ShaderFile& shaderFile)
    {
        // Open the file to see if it exists.
        std::ifstream inStream{data.filename};
        std::stringstream outString;

        if (!inStream)
        {
            auto message =
                fmt::format("Could not open file: {}", data.filename);
            throw std::runtime_error{message.c_str()};
        }

        // Check to see if this is the first time that we are adding something.
        // If it is, then we add the root file to create the hierarchy.
        if (shaderFile.includedFiles.empty())
        {
            auto timestamp = getFileLastWrite(data.filename);
            shaderFile.includedFiles.emplace_back(data.filename, -1,
                                                  data.fileKey, timestamp);
        }

        int fileNum = static_cast<int>(shaderFile.includedFiles.size() - 1);
        std::size_t lineNum = 1;
        std::string line;

        while (std::getline(inStream, line))
        {
            // Check to see if the line is a #version. If it is, then we must
            // skip it while increasing the line counter.
            if (line.find("#version") != std::string::npos)
            {
                outString << line + "\n";
                ++lineNum;
                continue;
            }

            // Check to see if the line is an include.
            if (line.find("#include") != std::string::npos)
            {
                // We have an include, so extract the path of the included file.
                std::string const includeStr = "#include";
                std::size_t pathSize = line.size() - includeStr.size() - 2;
                std::string path = line.substr(includeStr.size() + 1, pathSize);

                // Add the file to the list of includes.
                auto timestamp = getFileLastWrite(path);
                std::hash<std::string> stringHash;
                shaderFile.includedFiles.emplace_back(
                    path, fileNum, stringHash(path), timestamp);

                // Now recurse on the included file.
                auto parsedFile = readShaderSource(
                    shaderFile.includedFiles.back(), shaderFile);

                outString << parsedFile;
                continue;
            }

            // Create the #line directive.
            std::string lineInfo = "#line " + std::to_string(lineNum) + " " +
                                   std::to_string(data.fileKey) + "\n";
            outString << lineInfo;
            outString << line + "\n";
            ++lineNum;
        }

        return outString.str();
    }
} // namespace atlas::glx
