# GLX Module

The GLX module (short-hand for GL extensions) adds several utilities to
streamline common workflows with OpenGL. The components of the module are split
across 4 headers, which we will cover next.

## Buffer (`buffer.hpp`)

Contains a series of `constexpr` functions that can be used when detailing
buffer data for OpenGL. The main purpose for these functions is to provide easy
conversions from common types into the types that OpenGL expects, handling any
and all required casts. The functions are all templated to allow for any type of
data that can be sent into the buffers. These functions are:

* `buffer_offset`: used to compute offsets into a data buffer.
* `stride`: used to determine the stride to the next element in the buffer.
* `size`: used to determine the size of a buffer.
* `relative_offset`: computes an offset relative to the beginning of the buffer.

Here are some examples:

```c++
#include <atlas/glx/buffer.hpp>

std::vector<float> data;

// Create a VBO and send some data over.
GLuint vbo;
glCreateBuffers(1, &vbo);
glNamedBufferStorage(vbo, atlas::glx::size<float>(vertices.size()), vertices.data(), 0);

// ...

// Setup the attribute pointers for the VAO. In this case, we need a stride of
// 6 floats and we start at the beginning of the buffer.
glVErtexAttribPointer(0, 3, GL_FLOAT, atlas::glx::stride<float>(6),
atlas::glx::buffer_offset<float>(0));
```

## Context (`context.hpp`)

Defines all of the necessary structures to create a window using GLFW and an
OpenGL context. In order to access the functions, use

```c++
#include <atlas/glx/context.hpp>
```

> **Note:** All examples below assume that this header has been included. It
> will be omitted to avoid redundancy.

The header can be roughly divided into two groups: the GLFW
related functions and those that deal with OpenGL. We will look at the ones
dealing with GLFW first. Note that Atlas merely wraps GLFW in order to
streamline certain workflows, but makes no effort to change or interfere with
the interfaces that GLFW provides. As a result, it would be a good idea to read
the corresponding documentation from GLFW
[here](https://www.glfw.org/docs/latest/).

> **Note:** This guide is does *not* cover all of the steps required to setup a
> window with GLFW. It merely documents the functions that Atlas provides to
> simplify the process.

### GLFW-related Functions and Structures

The first stage to setting up a GLFW window is to initialize GLFW and passing in
an error function handler. This function will be invoked in the case where GLFW
encounters an error. The function can be used as follows:

```c++
// The error handler must have this signature
void error_handler(int code, char const* message)
{
    // Handle error here.
}

// ...

if (!atlas::glx::initialize_glfw(error_handler))
{
    // GLFW failed.
}
```

Once GLFW has been initialized, we can create our window. In order to do this,
we must first fill in the `WindowSettings` struct which will contain all of the
relevant information regarding the behaviour and properties of the window. Once
the struct is filled in, we can now create the new window as follows:

```c++
atlas::glx::WindowSettings settings;

// Determines the OpenGL version to use. Defaults to 4.5
settings.version = atlas::glx::ContextVersion{4, 5};

// Determines the dimensions of the window.
settings.size = atlas::glx::WindowSize{300, 300};

// Sets the window title. Defaults to the empty string.
settings.title   = "Sample Window";

// Determines the OpenGL profile to use. See the GLFW documentation for all
// available options. Defaults to GLFW_OPENGL_CORE_PROFILE
settings.profile = GLFW_OPENGL_CORE_PROFILE;

// Enables the creation of an OpenGL debug context. It is enabled by default on
// debug builds.
settings.enable_debug_contenxt = true;

// Disables any legacy compatibilities with OpenGL (fixed-function pipeline). 
// Defaults to true.
settings.is_forward_compatible = true;

// Determines if the window will be shown as maximized. If this is true, the 
// window size will be ignored. Defaults to false.
settings.is_maximized = false;

// Determines if the window will be resizeable. Defaults to true.
settings.is_resizeable = true;

// Determines if the window will be fullscreen. If this is true, the window size
// will be ingored. Defaults to false.
settings.is_fullscreen = false;

auto window = create_glfw_window(settings);
if (window == nullptr)
{
    // Window creation failed.
}
```

Once the window is ready, we can now bind the callback handlers that will be in
charge of dealing with any inputs we require. Internally, Atlas registers its
own callbacks and binds them to the provided window. It then passes a pointer to
a `WindowCallbacks` struct which contains all of the callback handlers we need.
When a new input comes in, the corresponding handler from Atlas is invoked, and
then it will forward the inputs to the corresponding function in the struct (if
there is one). You may override the default Atlas handlers by binding your own
functions as per the GLFW documentation.

The only important note to keep in mind is that for the key press callback,
Atlas defaults to closing the window when the ESC key is pressed. The function
binding can be done as follows:

```c++
auto mouse_press_callback =
    [](int button, int action, int mode, double, double) {
        // Handle the mouse press here.
    };

auto mouse_scroll_callback = [](double x_offset, double y_offset) {
    // Handle mouse scroll here.
};

auto key_press_callback =
    [](int key, int scancode, int action, int mods) {
        // Handle key press here.
    };

atlas::glx::WindowCallbacks callbacks;
callbacks.mouse_press_callback  = mousePressCallback;
callbacks.mouse_scroll_callback = mouseScrollCallback;
callbacks.key_press_callback    = keyPressCallback;

atlas::glx::bind_window_callbacks(window, callbacks);
```

At the end of our application, we need to destroy the window and clear out the
GLFW state. This can be done as follows:

```c++
destroy_glfw_window(window);
terminate_glfw();
```

### OpenGL-related Functions and Structures

Once we have a GLFW window created, we need to attach an OpenGL context to it.
This can be accomplished by filling in the `ContextVersion` struct with the
numbers for the OpenGL version we want as follows:

```c++
// Determines the OpenGL version to use. Defaults to 4.5
atlas::glx::ContextVersion version{4, 5};

if (!create_gl_context(window, version))
{
    // Failed to create OpenGL context.
}
```

## Error Callback (`error_callback.hpp`)

OpenGL 4.3 OpenGL introduced the ability to provide a callback function to
forward GL errors and handle them as needed. Atlas provides a custom callback
function that performs the following:

* Filters the error messages as specified when the function is setup.
* Prints the error messages to `stderr`.

The filtering for the error messages is accomplished through 3 `enum class`
types with contain the codes for all the OpenGL errors. These can be combined
into a custom bit mask as follows:

```c++
// Needed for bit-wise operators on enum classes.
#include <magic_enum.h>

// Needed for out-of-box operators.
using namespace magic_enum::bitwise_operators;
atlas::glx::initialize_gl_error_callback(
    atlas::glx::ErrorSource::all,           // Errors from all sources.
    atlas::glx::ErrorType::all,             // All types of errors.
    atlas::glx::ErrorSeverity::high | 
    atlas::glx::ErrorSeverity::medium);     // Only high and medium errors allowed.
```

For further information on the sources, types, and severities, please see the
declaration of the `ErrorSource`, `ErrorType`, and `ErrorSeverity` enums.

## GLSL (`glsl.hpp`)

This header contains several functions related to the compilation and linking of
GLSL shaders. It also provides several extensions and convenience functions.
