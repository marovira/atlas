#include <atlas/glx/Context.hpp>

#include <fmt/printf.h>

#include <catch2/catch.hpp>

#if defined(ATLAS_BUILD_GL_TESTS)
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

    std::vector<bool> callbacksSuccess(8, false);

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
    auto charCallback = [&callbacksSuccess](unsigned int) {
        callbacksSuccess[6] = true;
    };
    auto windowCloseCallback = [&callbacksSuccess](unsigned int) {
        callbacksSuccess[7] = true;
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

TEST_CASE("Checking callbacks on multiple windows", "[glx]")
{
    initializeGLFW(errorCallback);
    WindowSettings settings;
    auto window1 = createGLFWWindow(settings);
    auto window2 = createGLFWWindow(settings);

    bool callback1{false}, callback2{false};
    auto mousePressCallback1 = [&callback1](int, int, int) {
        fmt::print("In callback 1\n");
        callback1 = true;
    };
    auto mousePressCallback2 = [&callback2](int, int, int) {
        fmt::print("In callback 2\n");
        callback2 = true;
    };

    WindowCallbacks cb1, cb2;
    cb1.mousePressCallback = mousePressCallback1;
    cb2.mousePressCallback = mousePressCallback2;

    bindWindowCallbacks(window1, cb1);
    bindWindowCallbacks(window2, cb2);

    glfwMakeContextCurrent(window1);
    REQUIRE(createGLContext(window1, {4, 5}));

    glfwMakeContextCurrent(window2);
    REQUIRE(createGLContext(window2, {4, 5}));

    bool close1{false}, close2{false};
    while (!(close1 && close2))
    {
        {
            close1 = glfwWindowShouldClose(window1);
            glfwMakeContextCurrent(window1);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glfwSwapBuffers(window1);
        }

        {
            close2 = glfwWindowShouldClose(window2);
            glfwMakeContextCurrent(window2);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            glfwSwapBuffers(window2);
        }

        glfwPollEvents();
    }

    REQUIRE(callback1);
    REQUIRE(callback2);

    destroyGLFWWindow(window1);
    destroyGLFWWindow(window2);
    terminateGLFW();
}
#endif
