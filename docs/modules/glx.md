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
GLSL shaders. It also provides several extensions to the behaviour of GLSL to
allow for more streamlined operations. The main features are:

* Streamlined compilation and linking of shaders,
* Ability to `#include` files in GLSL code,
* Ability to reload live GLSL code.

The descriptions for each are outlined in the sections below. In order to use
any of these functions, use

```c++
#include <atlas/glx/glsl.hpp>
```

### Compiling and Linking Shaders

A shader can be compiled using `compile_shader`. The function will take in the
handle and source string for the shader and compile it. It will also handle all
errors and will return an `std::optional<std::string>`. If the return value is
empty, then compilation was successful, otherwise the returned string will
contain the error message from OpenGL. The function can be used as follows:

```c++
GLuint shader_handle{0};
auto result = atlas::glx::compile_shader(source_string, shader_handle);
if (!result)
{
    // Shader compilation failed.
    auto message = *result;
}
```

Similarly, shader linking is done with `link_shaders` which will take the
program handle. All error handling is done internally, and the return is the
same as `compile_shader`. It can be used as follows:

```c++
GLuint program_handle{0};
auto result = atlas::glx::link_shaders(program_handle);
if (!result)
{
    // Shader linking failed.
    auto message = *result;
}
```

### Using `#include` in GLSL

In order to use this feature, the loading of the shader source has to be managed
through `read_shader_source`. This function will take the name of the file, and
a list of include directories. The function will then read the source file line
by line and will transform it as follows:

```c++
#line <line-num> <file-num>
<code-line>
```

If the line contains a `#include`, then the parser will split out the name of
the included file and look for it in the included directories (note that the
current working directory is only tested if the list of include directories is
empty). It will then open the file and read its contents, pasting them into the
final source string. The `#line` directives are used to keep track of the line
number in the original file as well as which file is currently being read. Once
the file is fully processed, the function will return a `ShaderFile` struct.
This struct contains all of the needed information for the system to function
correctly. Its contents are:

* The name of the top shader file `filename`,
* The compiled source string `source_string`, and
* A list of files that are included in the top file `included_files`.

It is very important to not change `included_files` as it is used for other
operations. This list will also contain the time stamp of the file, which can be
used for live-reloading of shaders. The `source_string` variable can then be
used as an argument to `compile_shader`. 

Let's consider the following simple GLSL file `simple.glsl`:

```glsl
#version 450 core

layout(location = 0) in vec4 pos;

#include "uniform_matrices.glsl"

void main()
{
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
```

The file `uniform_matrices.glsl` contains the following:

```glsl
#ifndef UNIFORM_MATRICES_GLSL
#define UNIFORM_MATRICES_GLSL

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

#endif
```

We can then use `read_shader_source` as follows (assuming both files are in the
same directory):

```c++
std::string filename{"/path/to/simple.glsl"};
std::vector<std::string> include_dirs{"/path/to/"};
atlas::glx::ShaderFile shader_source = atlas::glx::read_shader_source(filename, include_dirs);
```

The resulting shader source string will look like this:

```glsl
#version 450 core
#line 2 0

#line 3 0
layout(location = 0) in vec4 pos;
#line 4 0

#line 1 1
#ifndef UNIFORM_MATRICES_GLSL
#line 2 1
#define UNIFORM_MATRICES_GLSL
#line 3 1

#line 4 1
layout (std140, binding = 0) uniform Matrices
#line 5 1
{
#line 6 1
    mat4 projection;
#line 7 1
    mat4 view;
#line 8 1
};
#line 9 1

#line 10 1
uniform mat4 model;
#line 11 1

#line 12 1
#endif
#line 6 0

#line 7 0
void main()
#line 8 0
{
#line 9 0
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
#line 10 0
}
```

A couple of notes regarding the `#include` directive:

* The only supported syntax for including files is `#include "filename.glsl"`.
* Atlas is designed to avoid circular includes (A includes B which includes A),
  as well as multiple includes in a single file.
* The top file **must** start with either a comment or a `#version` directive as
  per the GLSL standards.
* You are responsible for header guards to ensure that variables aren't defined
  multiple times.

Since the generated source string for the shader is more complex to read, error
messages will be more cryptic. To simplify this, Atlas provides a function that
will parse the error string and return an output similar to the one that a C/C++
compiler might return. To use it, do the following:

```c++
atlas::glx::ShaderFile file = atlas::glx::read_shader_source(filename, include_dirs);
auto result = atlas::glx::compile_shader(file.source_string, handle);
if (!result)
{
    std::string message = atlas::glx::parse_error_log(file, *result);
    // Print the formatted message.
}
```

It is important to note that currently the only types of messages that Atlas
recognizes are ones generated from NVidia cards. If Atlas is unable to recognize
the error formatting, it will return the string unchanged and will issue a
warning message.

> **Note:** The reason for this is that error messages are not standard across
> GPU vendors and the only types of GPUs I have access to are NVidia cards. If
> you wish to contribute to the project and have either an Intel or AMD card,
> please consider opening an issue providing the formatting of the error
> messages generated by those cards.

The other important thing to keep in mind is that the error message parsing will
**only** work for shader compilation, as linking strips away any means of
identifying which file(s) caused the error.

### Live-reloading of Shaders

Atlas provides a system for detecting whether a shader file has changed, and if
it has it will do the following:

* It will reload and parse the source string (including any `#include`
  directives),
* Compile the shaders,
* Re-link the shader program.

If any stage of this process fails, Atlas will leave the original shader program
untouched and will simply output the error messages. Note that in order for this
to work, your shader needs to have been loaded with `read_shader_source` as it
will use the time stamps contained in the `included_files` list. You can check
whether a shader needs to be reloaded and live-reload it as follows:

```c++
atlas::glx::ShaderFile file;
GLuint program_handle{0}, shader_handle{0};
std::vector<std::string> include_dirs;

// ...

if (atlas::glx::should_shader_be_reloaded(file))
{
    // Shader source changed, reload the shader.
    if (reload_shader(program_handle, shader_handle, file, include_dirs))
    {
        // Reload successful.
    }
    else
    {
        // Reload failed.
    }
}
```
