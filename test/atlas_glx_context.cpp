#include <atlas/glx/Context.hpp>

#include <fmt/printf.h>

#include <catch2/catch.hpp>

#if defined(ATLAS_BUILD_GL_TESTS)
static void error_callback(int code, char const* message)
{
    fmt::print("error ({}):{}\n", code, message);
}

using namespace atlas::glx;

TEST_CASE("[glx] - single window, single context")
{
    REQUIRE(initialize_glfw(error_callback));

    WindowSettings settings;
    auto window = create_glfw_window(settings);
    REQUIRE(window != nullptr);

    glfwMakeContextCurrent(window);

    REQUIRE(create_gl_context(window, settings.version));

    destroy_glfw_window(window);
    terminate_glfw();
}

TEST_CASE("[glx] - callbacks on single window")
{
    initialize_glfw(error_callback);
    WindowSettings settings;
    auto window = create_glfw_window(settings);

    std::vector<bool> callbacks_success(7, false);

    auto mouse_press_callback =
        [&callbacks_success](int, int, int, double, double) {
            callbacks_success[0] = true;
        };
    auto mouse_move_callback = [&callbacks_success](double, double) {
        callbacks_success[1] = true;
    };
    auto mouse_scroll_callback = [&callbacks_success](double, double) {
        callbacks_success[2] = true;
    };
    auto key_press_callback = [&callbacks_success](int, int, int, int) {
        callbacks_success[3] = true;
    };
    auto window_size_callback = [&callbacks_success](int, int) {
        callbacks_success[4] = true;
    };
    auto framebuffer_size_callback = [&callbacks_success](int, int) {
        callbacks_success[5] = true;
    };
    auto window_close_callback = [&callbacks_success](unsigned int) {
        callbacks_success[6] = true;
    };

    WindowCallbacks callbacks{mouse_press_callback,
                              mouse_move_callback,
                              mouse_scroll_callback,
                              key_press_callback,
                              window_size_callback,
                              framebuffer_size_callback,
                              window_close_callback};

    bind_window_callbacks(window, callbacks);
    glfwMakeContextCurrent(window);
    create_gl_context(window, settings.version);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool result{true};
    for (auto res : callbacks_success)
    {
        result &= res;
    }

    REQUIRE(result == true);

    destroy_glfw_window(window);
    terminate_glfw();
}

TEST_CASE("[glx] - callbacks on multiple windows")
{
    initialize_glfw(error_callback);
    WindowSettings settings;
    auto window1 = create_glfw_window(settings);
    auto window2 = create_glfw_window(settings);

    bool callback1{false}, callback2{false};
    auto mouse_press_callback1 = [&callback1](int, int, int, double, double) {
        fmt::print("In callback 1\n");
        callback1 = true;
    };
    auto mouse_press_callback2 = [&callback2](int, int, int, double, double) {
        fmt::print("In callback 2\n");
        callback2 = true;
    };

    WindowCallbacks cb1, cb2;
    cb1.mouse_press_callback = mouse_press_callback1;
    cb2.mouse_press_callback = mouse_press_callback2;

    bind_window_callbacks(window1, cb1);
    bind_window_callbacks(window2, cb2);

    glfwMakeContextCurrent(window1);
    REQUIRE(create_gl_context(window1, {4, 5}));

    glfwMakeContextCurrent(window2);
    REQUIRE(create_gl_context(window2, {4, 5}));

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

    destroy_glfw_window(window1);
    destroy_glfw_window(window2);
    terminate_glfw();
}
#endif
