#include "atlas/glx/GLSL.hpp"

#include <atlas/core/FMT.hpp>
#include <atlas/core/Platform.hpp>

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>

#if defined(ATLAS_PLATFORM_WINDOWS)
namespace fs = std::experimental::filesystem;
#else
namespace fs = std::filesystem;
#endif

namespace atlas::glx
{
    std::time_t getFileLastWrite(std::string const& filename)
    {
        fs::path filePath{filename};
        auto ftime = fs::last_write_time(filePath);
        return decltype(ftime)::clock::to_time_t(ftime);
    }

    std::string
    recurseOnShaderFiles(std::string const& filename, ShaderFile& file,
                         std::vector<std::string> const& includeDirectories);

    ShaderFile
    readShaderSource(std::string const& filename,
                     std::vector<std::string> const& includeDirectories)
    {
        ShaderFile file;

        // For the sake of uniformity, convert the file paths to the preferred
        // system style.
        fs::path p{filename};
        p = p.make_preferred();
        file.sourceString =
            recurseOnShaderFiles(p.string(), file, includeDirectories);
        return file;
    }

    std::string
    recurseOnShaderFiles(std::string const& filename, ShaderFile& file,
                         std::vector<std::string> const& includeDirectories)
    {
        std::ifstream inFile{filename};
        std::stringstream outString;

        if (!inFile)
        {
            auto message = fmt::format("Could not open file: {}.", filename);
            throw std::runtime_error{message.c_str()};
        }

        // Check to see if this is the first time we are adding something. If it
        // is, then we add the root file to help us build a hierarchy of
        // includes.
        if (file.includedFiles.empty())
        {
            auto timestamp = getFileLastWrite(filename);
            file.includedFiles.emplace_back(filename, -1, timestamp);
        }

        int fileNum = static_cast<int>(file.includedFiles.size()) - 1;
        int lineNum = 1;
        std::string line;
        while (std::getline(inFile, line))
        {
            // Check to see if the line is a #version. If it is, then we must
            // skip it while increasing the line counter.
            if (line.find("#version") != std::string::npos)
            {
                outString << line + "\n";
                ++lineNum;
                continue;
            }

            // Check to see if the line is an #include
            if (line.find("#include") != std::string::npos)
            {
                // We have an include, so extract the path of the included file.
                const std::string includeStr = "#include ";
                std::size_t pathSize = line.size() - includeStr.size() - 2;
                std::string path = line.substr(includeStr.size() + 1, pathSize);

                // Compute the absolute path.
                std::string absolutePath;
                if (includeDirectories.empty())
                {
                    // If we are not given an include directory, grab the
                    // directory of the current file.
                    fs::path p{filename};
                    auto baseDir = p.parent_path();
                    baseDir /= path;
                    absolutePath = baseDir.string();
                }
                else
                {
                    // We loop through all the include directories, testing
                    // to see if the file is valid.
                    for (auto& includeDir : includeDirectories)
                    {
                        auto tmpPath = includeDir + path;
                        if (fs::exists(tmpPath))
                        {
                            absolutePath = tmpPath;
                            break;
                        }
                    }

                    // If the path is still empty, we found nothing, so throw
                    // an error.
                    if (absolutePath.empty())
                    {
                        auto message = fmt::format(
                            "In file {}({}): Cannot open include file: \'{}\': "
                            "No such file or directory",
                            filename, lineNum + 1, path);
                        throw std::runtime_error{message};
                    }
                }

                auto timestamp = getFileLastWrite(absolutePath);
                FileData f{absolutePath, fileNum, timestamp};

                // Check if we have seen this file before to prevent circular
                // includes.

                auto res = std::find_if(file.includedFiles.begin(),
                                        file.includedFiles.end(),
                                        [f](FileData const& other) -> bool {
                                            return f.filename == other.filename;
                                        });
                if (res != file.includedFiles.end())
                {
                    // We have seen this file before, so do nothing.
                    continue;
                }

                file.includedFiles.emplace_back(absolutePath, fileNum,
                                                timestamp);
                auto parsedFile = recurseOnShaderFiles(absolutePath, file,
                                                       includeDirectories);

                outString << parsedFile;
                continue;
            }

            // Create the #line directive.
            std::string lineInfo =
                fmt::format("#line {} {}\n", lineNum, fileNum);
            outString << lineInfo;
            outString << line + "\n";
            ++lineNum;
        }

        return outString.str();
    }

} // namespace atlas::glx
