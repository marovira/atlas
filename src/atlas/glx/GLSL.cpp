#include "GLSL.hpp"

#include <atlas/core/Filesystem.hpp>
#include <atlas/core/Platform.hpp>
#include <fmt/printf.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>

namespace fs = std::experimental::filesystem;

namespace atlas::glx
{
    std::string
    recurseOnShaderFiles(std::string const& filename,
                         ShaderFile& file,
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
        if (!fs::exists(p))
        {
            auto message = fmt::format(
                "error: no such file or directory: \'{}\'.\n", filename);
            throw std::runtime_error{message};
        }
        file.sourceString =
            recurseOnShaderFiles(p.string(), file, includeDirectories);
        file.filename = p.string();
        return file;
    }

    bool shouldShaderBeReloaded(ShaderFile const& file)
    {
        for (auto& unit : file.includedFiles)
        {
            std::time_t stamp = core::getFileLastWrite(unit.filename);
            double secs       = std::difftime(stamp, unit.lastWrite);
            if (secs > 0)
            {
                return true;
            }
        }

        return false;
    }

    std::string
    recurseOnShaderFiles(std::string const& filename,
                         ShaderFile& file,
                         std::vector<std::string> const& includeDirectories)
    {
        std::ifstream inFile{filename};
        std::stringstream outString;

        if (!inFile)
        {
            fmt::print(stderr,
                       "error: no such file or directory: \'{}\'.\n",
                       filename);
            return {};
        }

        // Check to see if this is the first time we are adding something. If it
        // is, then we add the root file to help us build a hierarchy of
        // includes. Additionally, if this is the first file, then by
        // rule it must contain the #version directive, so set the
        // found variable to false so we know when we've found it and can
        // ignore any lines before it to avoid compiler errors.
        bool foundVersionDirective{true};
        if (file.includedFiles.empty())
        {
            auto timestamp = core::getFileLastWrite(filename);
            file.includedFiles.emplace_back(filename, -1, timestamp);
            foundVersionDirective = false;
        }

        int fileNum = static_cast<int>(file.includedFiles.size()) - 1;
        int lineNum = 1;
        std::string line;
        bool inCComment{false};
        while (std::getline(inFile, line))
        {
            // Check to see if the line is a comment. If it is, then skip it
            // while increasing the line counter.
            // Do this check first to accommodate for the case where there are
            // comments before the #version directive.
            if (line.find("//") != std::string::npos)
            {
                outString << line + "\n";
                ++lineNum;
                continue;
            }

            // Found a starting C-style comment, so we simply skip the
            // line while increasing the counter. This will continue until
            // we find a terminating symbol.
            if (line.find("/*") != std::string::npos)
            {
                inCComment = true;
                outString << line + "\n";
                ++lineNum;
                continue;
            }

            // Found the closing symbol for the C-style comment, so parsing
            // can resume as normal.
            if (line.find("*/") != std::string::npos)
            {
                inCComment = false;
                outString << line + "\n";
                ++lineNum;
                continue;
            }

            // If we are in the middle of a C-style comment, skip the lines
            // while increasing the line counter.
            if (inCComment)
            {
                outString << line + "\n";
                ++lineNum;
                continue;
            }

            // Check to see if the line is a #version. If it is, then we must
            // skip it while increasing the line counter.
            if (line.find("#version") != std::string::npos)
            {
                outString << line + "\n";
                ++lineNum;
                foundVersionDirective = true;
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

                    if (absolutePath.empty())
                    {
                        fmt::print(
                            stderr,
                            "In file {}({}): Cannot open include file: \'{}\': "
                            "No such file or directory.\n",
                            filename,
                            lineNum + 1,
                            path);
                        continue;
                    }
                }

                auto timestamp = core::getFileLastWrite(absolutePath);
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
                    // We have seen this file before, so do nothing while
                    // increasing the line counter.
                    ++lineNum;
                    continue;
                }

                file.includedFiles.emplace_back(
                    absolutePath, fileNum, timestamp);
                auto parsedFile = recurseOnShaderFiles(
                    absolutePath, file, includeDirectories);

                outString << parsedFile;

                // Since we are removing the #include directive from the code,
                // we must increase the line counter to ensure that the next
                // line we parse is correct.
                ++lineNum;
                continue;
            }

            // If we haven't found the version directive yet, do not add any
            // #line directives as this will result in a compiler error.
            // Instead, sip the line while increasing the line counter.
            if (!foundVersionDirective)
            {
                outString << line + "\n";
                ++lineNum;
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

    std::optional<std::string> compileShader(std::string const& source,
                                             GLuint handle)
    {
        // If there's no source, do nothing.
        if (source.empty())
        {
            return {};
        }

        auto glSource = static_cast<GLchar const*>(source.c_str());
        glShaderSource(handle, 1, &glSource, nullptr);
        glSource = nullptr;

        glCompileShader(handle);

        GLint compiled;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
        if (compiled == 0)
        {
            GLsizei len;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

            auto log = new GLchar[len + 1];
            glGetShaderInfoLog(handle, len, &len, log);
            std::string errorMessage{log};
            delete[] log;

            return {errorMessage};
        }

        return {};
    }

    std::optional<std::string> linkShaders(GLuint handle)
    {
        if (handle == 0u)
        {
            return {};
        }

        glLinkProgram(handle);

        GLint linked;
        glGetProgramiv(handle, GL_LINK_STATUS, &linked);
        if (linked == 0)
        {
            GLsizei len;
            glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);

            auto log = new GLchar[len + 1];
            glGetProgramInfoLog(handle, len, &len, log);
            std::string errorMessage{log};
            delete[] log;

            return {errorMessage};
        }

        return {};
    }

    std::string parseErrorLog(ShaderFile const& file, std::string const& log)
    {
        // If the log is empty, do nothing.
        if (log.empty())
        {
            return {};
        }

        // It appears that this type of format only works on NVidia cards, so
        // check to see if we're in an NVida card. If we aren't, return the
        // log as is and output a message.
        std::string vendorID =
            reinterpret_cast<char const*>(glGetString(GL_VENDOR));
        if (vendorID.find("NVIDIA") == std::string::npos)
        {
            fmt::print(stderr, "warning: unsupported vendor.\n");
            fmt::print(stderr, "warning: error log is unchanged.\n");
            return log;
        }

        std::stringstream logStream{log};
        std::stringstream outLog;
        std::string line;

        while (std::getline(logStream, line, '\n'))
        {
            std::smatch match;
            std::regex pattern(R"(\d+\(\d*\))");

            // We are only interested in the first match.
            std::regex_search(line, match, pattern);

            if (match.size() != 1)
            {
                // This should not be happening!
                fmt::print(stderr,
                           "warning: unrecognized OpenGL error syntax.\n");
                fmt::print(stderr, "warning: error log is unchanged.\n");
                return log;
            }

            std::string info  = match[0];
            std::string error = match.suffix();
            error             = error.substr(1, error.size() - 1);

            // Now extract the numbers by locating ().
            std::size_t start = info.find("(");
            std::size_t end   = info.find(")");
            std::size_t delta = end - start;

            // Grab the numbers;
            int fileNum = std::stoi(info.substr(0, start));
            int lineNum = std::stoi(info.substr(start + 1, delta));

            // Now assemble the include hierarchy for the file.
            int parent = file.includedFiles[fileNum].parent;
            std::vector<int> hierarchy;
            hierarchy.push_back(fileNum);
            while (parent != -1)
            {
                hierarchy.push_back(parent);
                parent = file.includedFiles[parent].parent;
            }

            // Check if the hierarchy has size 1. If it does, then the error
            // message was generated in the top file, and we just need to print
            // out the error.
            if (hierarchy.size() == 1)
            {
                auto message =
                    fmt::format("In file {}({}): {}\n",
                                file.includedFiles[hierarchy[0]].filename,
                                lineNum,
                                error);
                outLog << message;
                continue;
            }

            // Otherwise, we have to generate the include hierarchy messages.
            // To do this, we traverse the list in reverse.
            for (std::size_t i{hierarchy.size() - 1}; i > 0; --i)
            {
                auto message =
                    fmt::format("In file included from {}:\n",
                                file.includedFiles[hierarchy[i]].filename);
                outLog << message;
            }

            auto message = fmt::format(
                "{}({}): {}\n", file.includedFiles[0].filename, lineNum, error);
            outLog << message;
        }

        return outLog.str();
    }

    bool reloadShader(GLuint programHandle,
                      GLuint shaderHandle,
                      ShaderFile& file,
                      std::vector<std::string> const& includeDirectories)
    {
        file = readShaderSource(file.filename, includeDirectories);
        if (auto res = glx::compileShader(file.sourceString, shaderHandle); res)
        {
            auto message = parseErrorLog(file, res.value());
            fmt::print(stderr, "error: {}\n", message);
            return false;
        }

        if (auto res = glx::linkShaders(programHandle); res)
        {
            auto message = parseErrorLog(file, res.value());
            fmt::print(stderr, "error: {}\n", message);
            glDetachShader(programHandle, shaderHandle);
            return false;
        }

        return true;
    }
} // namespace atlas::glx
