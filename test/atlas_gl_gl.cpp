#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <catch2/catch.hpp>

TEST_CASE("Checking OpenGL versions", "[gl]")
{
    REQUIRE(glfwInit() != 0);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(100, 100, "Test", NULL, NULL);
    REQUIRE(window != nullptr);
    if (window != nullptr)
    {
        glfwMakeContextCurrent(window);
        REQUIRE(gl3wInit() == 0);
        REQUIRE(gl3wIsSupported(4, 5) != 0);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}