#include <atlas/glx/Context.hpp>

#include <atlas/core/FMT.hpp>

#include <catch2/catch.hpp>

static void errorCallback(int code, char const* message)
{
    fmt::print("error ({}):{}\n", code, message);
}

using namespace atlas::glx;

TEST_CASE("Checking single window, single context", "[glx]")
{
    REQUIRE(initializeGLFW(errorCallback));

    WindowSettings settings;
    auto window = createGLFWWindow(settings);
    REQUIRE(window != nullptr);

    glfwMakeContextCurrent(window);

    REQUIRE(createGLContext(window, settings.version));

    destroyGLFWWindow(window);
    terminateGLFW();
}

TEST_CASE("Checking callbacks on single window", "[glx]")
{
    initializeGLFW(errorCallback);
    WindowSettings settings;
    auto window = createGLFWWindow(settings);

    std::vector<bool> callbacksSuccess(7, false);

    auto mousePressCallback = [&callbacksSuccess](int, int, int) {
        callbacksSuccess[0] = true;
    };
    auto mouseMoveCallback = [&callbacksSuccess](double, double) {
        callbacksSuccess[1] = true;
    };
    auto mouseScrollCallback = [&callbacksSuccess](double, double) {
        callbacksSuccess[2] = true;
    };
    auto keyPressCallback = [&callbacksSuccess](int, int, int, int) {
        callbacksSuccess[3] = true;
    };
    auto windowSizeCallback = [&callbacksSuccess](int, int) {
        callbacksSuccess[4] = true;
    };
    auto framebufferSizeCallback = [&callbacksSuccess](int, int) {
        callbacksSuccess[5] = true;
    };
    auto windowCloseCallback = [&callbacksSuccess]() {
        callbacksSuccess[6] = true;
    };

    WindowCallbacks callbacks{mousePressCallback,  mouseMoveCallback,
                              mouseScrollCallback, keyPressCallback,
                              windowSizeCallback,  framebufferSizeCallback,
                              windowCloseCallback};

    bindWindowCallbacks(window, callbacks);
    glfwMakeContextCurrent(window);
    createGLContext(window, settings.version);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool result{true};
    for (auto res : callbacksSuccess)
    {
        result &= res;
    }

    REQUIRE(result == true);

    destroyGLFWWindow(window);
    terminateGLFW();
}
