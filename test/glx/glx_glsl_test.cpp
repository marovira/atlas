#include "test_data_paths.hpp"
#include "test_expected_paths.hpp"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <atlas/glx/glsl.hpp>
#include <catch2/catch.hpp>
#include <fmt/printf.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <zeus/filesystem.hpp>
#include <zeus/platform.hpp>

using namespace atlas::glx;
namespace fs = std::experimental::filesystem;

namespace atlas::glx
{
    bool operator==(FileData const& lhs, FileData const& rhs)
    {
        return lhs.filename == rhs.filename && lhs.parent == rhs.parent &&
               lhs.last_write == rhs.last_write;
    }

    bool operator!=(FileData const& lhs, FileData const& rhs)
    {
        return !(lhs == rhs);
    }
} // namespace atlas::glx

std::string load_expected_string(std::string const& filename)
{
    std::ifstream in_stream{filename};
    std::stringstream out_string;

    if (!in_stream)
    {
        auto message =
            fmt::format("Could not open expected test file: {}", filename);
        throw std::runtime_error{message.c_str()};
    }

    std::string line;
    while (std::getline(in_stream, line))
    {
        out_string << line + "\n";
    }
    return out_string.str();
}

std::string normalize_path(std::string const& path)
{
    fs::path p{path};
    p = p.make_preferred();
    return p.string();
}

TEST_CASE("[glsl] - load_shader_file: non-existent file", "[glx]")
{
    try
    {
        auto result = read_shader_source("foo.glsl");
    }
    catch (std::runtime_error const& e)
    {
        std::string_view msg{e.what()};
        REQUIRE(msg.empty() != true);
    }
}

TEST_CASE("[glsl] - load_shader_file: empty file", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_empty_file]);
    auto result          = read_shader_source(filename);
    REQUIRE(result.source_string.empty() == true);
    REQUIRE(result.included_files.size() == 1);

    auto included_file = result.included_files.front();
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: single line", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_single_line]);
    std::string expected_filename{expected_files[glx_single_line_expected]};

    // Load the expected string.
    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 1);

    auto included_file = result.included_files.front();
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: simple file", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_simple_file]);
    std::string expected_filename{expected_files[glx_simple_file_expected]};

    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 1);

    auto included_file = result.included_files.front();
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: simple file with comments", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_simple_file_comments]);
    std::string expected_filename{
        expected_files[glx_simple_file_comments_expected]};

    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 1);

    auto included_file = result.included_files.front();
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: simple file with header comments",
          "[glx]")
{
    std::string filename = normalize_path(test_data[glx_header_comments]);
    std::string expected_filename{expected_files[glx_header_comments_expected]};

    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 1);

    auto included_file = result.included_files.front();
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: simple file with c-style comments",
          "[glx]")
{
    std::string filename = normalize_path(test_data[glx_c_comments]);
    std::string expected_filename{expected_files[glx_c_comments_expected]};

    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 1);

    auto included_file = result.included_files.front();
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: single include", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_single_include]);
    std::string expected_filename{expected_files[glx_single_include_expected]};
    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 2);

    auto included_file = result.included_files[0];
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[1];
    expected_file.filename = normalize_path(test_data[uniform_matrices]);
    expected_file.parent   = 0;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[uniform_matrices]);
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: multiple includes", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_multiple_includes]);
    std::string expected_filename{
        expected_files[glx_multiple_includes_expected]};
    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 3);

    auto included_file = result.included_files[0];
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[1];
    expected_file.filename = normalize_path(test_data[uniform_bindings]);
    expected_file.parent   = 0;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[uniform_bindings]);
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[2];
    expected_file.filename = normalize_path(test_data[uniform_matrices]);
    expected_file.parent   = 0;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[uniform_matrices]);
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: nested includes", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_nested_include]);
    std::string expected_filename{expected_files[glx_nested_include_expected]};
    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 4);

    auto included_file = result.included_files[0];
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[1];
    expected_file.filename = normalize_path(test_data[nested_include]);
    expected_file.parent   = 0;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[nested_include]);
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[2];
    expected_file.filename = normalize_path(test_data[uniform_bindings]);
    expected_file.parent   = 1;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[uniform_bindings]);
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[3];
    expected_file.filename = normalize_path(test_data[uniform_matrices]);
    expected_file.parent   = 1;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[uniform_matrices]);
    REQUIRE(included_file == expected_file);
}

TEST_CASE("[glsl] - load_shader_file: circular includes", "[glx]")
{
    std::string filename = normalize_path(test_data[glx_circular_include]);
    std::string expected_filename{
        expected_files[glx_circular_include_expected]};
    auto expectedString = load_expected_string(expected_filename);

    auto result = read_shader_source(filename);
    REQUIRE(result.source_string == expectedString);
    REQUIRE(result.included_files.size() == 3);

    auto included_file = result.included_files[0];
    FileData expected_file{filename, -1, zeus::get_file_last_write(filename)};
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[1];
    expected_file.filename = normalize_path(test_data[circular_include_a]);
    expected_file.parent   = 0;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[circular_include_a]);
    REQUIRE(included_file == expected_file);

    included_file          = result.included_files[2];
    expected_file.filename = normalize_path(test_data[circular_include_b]);
    expected_file.parent   = 1;
    expected_file.last_write =
        zeus::get_file_last_write(test_data[circular_include_b]);
    REQUIRE(included_file == expected_file);
}
