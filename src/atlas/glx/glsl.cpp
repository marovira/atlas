#include "glsl.hpp"

#include <fmt/printf.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <zeus/assert.hpp>
#include <zeus/filesystem.hpp>
#include <zeus/platform.hpp>

namespace fs = std::experimental::filesystem;

namespace atlas::glx
{
    std::string recurse_on_shader_files(std::string const& filename,
                                        ShaderFile& file,
                                        std::vector<std::string> const& include_dirs);

    ShaderFile read_shader_source(std::string const& filename,
                                  std::vector<std::string> const& include_dirs)
    {
        ShaderFile file;

        // For the sake of uniformity, convert the file paths to the preferred
        // system style.
        fs::path p{filename};
        p = p.make_preferred();
        if (!fs::exists(p))
        {
            auto message =
                fmt::format("error: no such file or directory: \'{}\'.\n", filename);
            throw std::runtime_error{message};
        }
        file.source_string = recurse_on_shader_files(p.string(), file, include_dirs);
        file.filename      = p.string();
        return file;
    }

    bool should_shader_be_reloaded(ShaderFile const& file)
    {
        for (auto& unit : file.included_files)
        {
            std::time_t stamp = zeus::get_file_last_write(unit.filename);
            double secs       = std::difftime(stamp, unit.last_write);
            if (secs > 0)
            {
                return true;
            }
        }

        return false;
    }

    std::string recurse_on_shader_files(std::string const& filename,
                                        ShaderFile& file,
                                        std::vector<std::string> const& include_dirs)
    {
        std::ifstream in_file{filename};
        std::stringstream out_string;

        if (!in_file)
        {
            fmt::print(stderr, "error: no such file or directory: \'{}\'.\n", filename);
            return {};
        }

        // Check to see if this is the first time we are adding something. If it
        // is, then we add the root file to help us build a hierarchy of
        // includes. Additionally, if this is the first file, then by
        // rule it must contain the #version directive, so set the
        // found variable to false so we know when we've found it and can
        // ignore any lines before it to avoid compiler errors.
        bool found_version_directive{true};
        if (file.included_files.empty())
        {
            auto timestamp = zeus::get_file_last_write(filename);
            file.included_files.emplace_back(filename, -1, timestamp);
            found_version_directive = false;
        }

        int file_num = static_cast<int>(file.included_files.size()) - 1;
        int line_num = 1;
        std::string line;
        bool in_c_comment{false};
        while (std::getline(in_file, line))
        {
            // Check to see if the line is a comment. If it is, then skip it
            // while increasing the line counter.
            // Do this check first to accommodate for the case where there are
            // comments before the #version directive.
            if (line.find("//") != std::string::npos)
            {
                out_string << line + "\n";
                ++line_num;
                continue;
            }

            // Found a starting C-style comment, so we simply skip the
            // line while increasing the counter. This will continue until
            // we find a terminating symbol.
            if (line.find("/*") != std::string::npos)
            {
                in_c_comment = true;
                out_string << line + "\n";
                ++line_num;
                continue;
            }

            // Found the closing symbol for the C-style comment, so parsing
            // can resume as normal.
            if (line.find("*/") != std::string::npos)
            {
                in_c_comment = false;
                out_string << line + "\n";
                ++line_num;
                continue;
            }

            // If we are in the middle of a C-style comment, skip the lines
            // while increasing the line counter.
            if (in_c_comment)
            {
                out_string << line + "\n";
                ++line_num;
                continue;
            }

            // Check to see if the line is a #version. If it is, then we must
            // skip it while increasing the line counter.
            if (line.find("#version") != std::string::npos)
            {
                out_string << line + "\n";
                ++line_num;
                found_version_directive = true;
                continue;
            }

            // Check to see if the line is an #include
            if (line.find("#include") != std::string::npos)
            {
                // We have an include, so extract the path of the included file.
                const std::string include_string = "#include ";
                std::size_t path_size = line.size() - include_string.size() - 2;
                std::string path      = line.substr(include_string.size() + 1, path_size);

                // Compute the absolute path.
                std::string absolute_path;
                if (include_dirs.empty())
                {
                    // If we are not given an include directory, grab the
                    // directory of the current file.
                    fs::path p{filename};
                    auto base_dir = p.parent_path();
                    base_dir /= path;
                    absolute_path = base_dir.string();
                }
                else
                {
                    // We loop through all the include directories, testing
                    // to see if the file is valid.
                    for (auto& include_dir : include_dirs)
                    {
                        auto temp_path = include_dir + path;
                        if (fs::exists(temp_path))
                        {
                            absolute_path = temp_path;
                            break;
                        }
                    }

                    if (absolute_path.empty())
                    {
                        fmt::print(stderr,
                                   "In file {}({}): Cannot open include file: \'{}\': "
                                   "No such file or directory.\n",
                                   filename,
                                   line_num + 1,
                                   path);
                        continue;
                    }
                }

                auto timestamp = zeus::get_file_last_write(absolute_path);
                FileData f{absolute_path, file_num, timestamp};

                // Check if we have seen this file before to prevent circular
                // includes.

                auto res = std::find_if(file.included_files.begin(),
                                        file.included_files.end(),
                                        [f](FileData const& other) -> bool {
                                            return f.filename == other.filename;
                                        });
                if (res != file.included_files.end())
                {
                    // We have seen this file before, so do nothing while
                    // increasing the line counter.
                    ++line_num;
                    continue;
                }

                file.included_files.emplace_back(absolute_path, file_num, timestamp);
                auto parsed_file =
                    recurse_on_shader_files(absolute_path, file, include_dirs);

                out_string << parsed_file;

                // Since we are removing the #include directive from the code,
                // we must increase the line counter to ensure that the next
                // line we parse is correct.
                ++line_num;
                continue;
            }

            // If we haven't found the version directive yet, do not add any
            // #line directives as this will result in a compiler error.
            // Instead, sip the line while increasing the line counter.
            if (!found_version_directive)
            {
                out_string << line + "\n";
                ++line_num;
                continue;
            }

            // Create the #line directive.
            std::string line_info = fmt::format("#line {} {}\n", line_num, file_num);
            out_string << line_info;
            out_string << line + "\n";
            ++line_num;
        }

        return out_string.str();
    }

    std::optional<std::string> compile_shader(std::string const& source, GLuint handle)
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

    std::optional<std::string> link_shaders(GLuint handle)
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

    std::string parse_error_log(ShaderFile const& file, std::string const& log)
    {
        // If the log is empty, do nothing.
        if (log.empty())
        {
            return {};
        }

        // It appears that this type of format only works on NVidia cards, so
        // check to see if we're in an NVida card. If we aren't, return the
        // log as is and output a message.
        std::string vendor_id = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
        if (vendor_id.find("NVIDIA") == std::string::npos)
        {
            fmt::print(stderr, "warning: unsupported vendor.\n");
            fmt::print(stderr, "warning: error log is unchanged.\n");
            return log;
        }

        std::stringstream log_stream{log};
        std::stringstream out_log;
        std::string line;

        while (std::getline(log_stream, line, '\n'))
        {
            std::smatch match;
            std::regex pattern(R"(\d+\(\d*\))");

            // We are only interested in the first match.
            std::regex_search(line, match, pattern);

            if (match.size() != 1)
            {
                // This should not be happening!
                fmt::print(stderr, "warning: unrecognized OpenGL error syntax.\n");
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
            int file_num = std::stoi(info.substr(0, start));
            int line_num = std::stoi(info.substr(start + 1, delta));

            // Now assemble the include hierarchy for the file.
            int parent = file.included_files[file_num].parent;
            std::vector<int> hierarchy;
            hierarchy.push_back(file_num);
            while (parent != -1)
            {
                hierarchy.push_back(parent);
                parent = file.included_files[parent].parent;
            }

            // Check if the hierarchy has size 1. If it does, then the error
            // message was generated in the top file, and we just need to print
            // out the error.
            if (hierarchy.size() == 1)
            {
                auto message = fmt::format("In file {}({}): {}\n",
                                           file.included_files[hierarchy[0]].filename,
                                           line_num,
                                           error);
                out_log << message;
                continue;
            }

            // Otherwise, we have to generate the include hierarchy messages.
            // To do this, we traverse the list in reverse.
            for (std::size_t i{hierarchy.size() - 1}; i > 0; --i)
            {
                auto message = fmt::format("In file included from {}:\n",
                                           file.included_files[hierarchy[i]].filename);
                out_log << message;
            }

            auto message = fmt::format("{}({}): {}\n",
                                       file.included_files[0].filename,
                                       line_num,
                                       error);
            out_log << message;
        }

        return out_log.str();
    }

    bool reload_shader(GLuint program_handle,
                       GLuint shader_handle,
                       ShaderFile& file,
                       std::vector<std::string> const& include_dirs)
    {
        file = read_shader_source(file.filename, include_dirs);
        if (auto res = glx::compile_shader(file.source_string, shader_handle); res)
        {
            auto message = parse_error_log(file, res.value());
            fmt::print(stderr, "error: {}\n", message);
            return false;
        }

        if (auto res = glx::link_shaders(program_handle); res)
        {
            auto message = parse_error_log(file, res.value());
            fmt::print(stderr, "error: {}\n", message);
            glDetachShader(program_handle, shader_handle);
            return false;
        }

        return true;
    }

    std::string get_shader_type_string(GLenum type)
    {
        if (type == GL_VERTEX_SHADER)
        {
            return "vertex shader";
        }
        else if (type == GL_GEOMETRY_SHADER)
        {
            return "geometry shader";
        }
        else if (type == GL_TESS_CONTROL_SHADER)
        {
            return "tessellation control shader";
        }
        else if (type == GL_TESS_EVALUATION_SHADER)
        {
            return "tessellation evaluation shader";
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            return "fragment shader";
        }
        else if (type == GL_COMPUTE_SHADER)
        {
            return "compute shader";
        }

        return "";
    }

    std::optional<std::string>
    create_separable_shader_program(GLenum type, GLuint program, ShaderFile const& file)
    {
        ASSERT(program != 0);

        GLuint shader = glCreateShader(type);

        if (!shader)
        {
            std::string shader_type = get_shader_type_string(type);
            std::string message = fmt::format("error: failed to create {}", shader_type);
            throw std::runtime_error{message};
        }

        if (auto result = compile_shader(file.source_string, shader); result)
        {
            auto message = parse_error_log(file, *result);
            return {message};
        }

        glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glAttachShader(program, shader);

        if (auto result = link_shaders(program); result)
        {
            return result;
        }

        glDetachShader(program, shader);
        glDeleteShader(shader);

        return {};
    }

    bool reload_separable_shader_program(GLenum type,
                                         GLuint program_handle,
                                         ShaderFile& file,
                                         std::vector<std::string> const& include_dirs)
    {
        file = read_shader_source(file.filename, include_dirs);
        if (auto result = create_separable_shader_program(type, program_handle, file);
            result)
        {
            fmt::print(stderr, "error: {}\n", *result);
            return false;
        }

        return true;
    }

} // namespace atlas::glx
