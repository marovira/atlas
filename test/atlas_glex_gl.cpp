#include <atlas/glex/GlL.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Checking OpenGL versions", "[glex]")
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    auto window = glfwCreateWindow(10, 10, "Test", NULL, NULL);
    REQUIRE(window != nullptr);
    REQUIRE(gl3wInit() == 0);
    REQUIRE(gl3wIsSupported(4, 5) != 0);

    glfwDestroyWindow(window);
    glfwTerminate();
}