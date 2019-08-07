#include "TestDataPaths.hpp"

#include <atlas/core/Platform.hpp>
#include <atlas/utils/LoadObjFile.hpp>

#include <catch2/catch.hpp>
#include <fmt/printf.h>

#include <experimental/filesystem>

using namespace atlas::utils;
namespace fs = std::experimental::filesystem;

struct CubeMesh
{
    std::string meshPath{TestDataRoot + std::string("obj/cube/cube.obj")};
    std::string mtlPath{TestDataRoot + std::string("obj/cube/default.mtl")};
};

struct SponzaMesh
{
    std::string meshPath{TestDataRoot + std::string("obj/sponza/sponza.obj")};
    std::string mtlPath{TestDataRoot + std::string("obj/sponza/sponza.mtl")};
};

struct TeapotMesh
{
    std::string meshPath{TestDataRoot + std::string("obj/teapot/teapot.obj")};
    std::string mtlPath{TestDataRoot + std::string("obj/teapot/default.mtl")};
};

TEST_CASE("Checking cube mesh", "[utils]")
{
    CubeMesh cube;

    REQUIRE(fs::exists(cube.meshPath));

    auto mesh = loadObjMesh(cube.meshPath);
}
