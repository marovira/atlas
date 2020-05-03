# GUI Module

Atlas bundles ImGUI as a way of creating and managing UI elements for graphics
development. It is recommended that you read the documentation and examples for
ImGUI [here](https://github.com/ocornut/imgui). As ImGUI does not provide on its
own all of the structure necessary for rendering, Atlas provides the functions
that perform these tasks. It is important to note that all of them rely on
OpenGL 4.5+, so you must support at least this version in order to use this
module.

There are two structs that contain all of the necessary data for the UI
management:

* `UIRenderData`: contains all required handles and rendering attributes for the
  UI.
* `UIWindowData`: contains all the window related data.

## Setting up the UI

The UI system is built with OpenGL and GLFW in mind, so it will fit right into
the related workflows within the framework. For the sake of brevity, we will
skip the process for creating a GLFW window and an OpenGL context. Also, to have
access to the functions, use:

```c++
#include <atlas/gui/gui.hpp>
```

### Callbacks

Before creating any UI components, the UI callback handlers need to be bound.
This can be done easily by calling them within your own callback functions and
just bind those to the window through `atlas::glx::bind_window_callbacks`. In
particular, the following callbacks *need* to be bound in order for the UI to
work correctly:

* `mouse_pressed_callback`,
* `key_press_callback`, and
* `char_callback`.

### Initializing the UI

Once the callbacks are bound, we can move on to setting up the UI state. This
should be done immediately after creating and binding the GL context to the GLFW
window. The first step involves creating the ImGUI context and optionally
setting the colour style:

```c++
ImGui::CreateContext();
ImGui::StyleColorsDark();
```

Once this is done, we can now setup the Atlas state:

```c++
atlas::glx::UIWindowData window_data;
atlas::glx::UIRenderData render_data;

atlas::gui::initialize_ui_window_data(render_data);
atlas::gui::initialize_ui_render_data(render_data);
```

The last setup step is to bind the current GLFW window to the window data, which
can be done as follows:

```c++
GLFWwindow* window;

atlas::gui::set_ui_window(window_data, window);
```

## Cleaning Up the UI

Once the program is about to exit, we can clean up the state related to the UI.
This should be done before the GLFW window is destroyed. To clean up, do the
following:

```c++
atlas::gui::destroy_ui_render_data(render_data);
atlas::gui::destroy_ui_window(window_data);
```

## Rendering the UI

To render the UI, the first thing that needs to happen is that a new frame must
be declared. This is done as follows:

```c++
atlas::gui::new_frame(window_data);

// Now start a new window.
ImGui::Begin("Window Title");
// More ImGUI commands.
ImGui::End();
```

The call to `new_frame` must happen *before* any calls to `ImGui` functions.
Once all UI elements have been defined, the next step is to render the UI and
end the frame:

```c++
ImGui::Render();
atlas::gui::end_frame(window_data, render_data);
```
