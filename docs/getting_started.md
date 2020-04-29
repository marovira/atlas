# Getting Started

Atlas uses CMake in order to manage and generate the build files. As a result,
there are two ways in which Atlas can be used:

* Adding the Atlas source directly to your project or,
* Using CMake to fetch the code directly.

Note that this document assumes familiarity with CMake.

## Dependencies

Before getting started, you must ensure that you have all dependencies
installed. In particular, you will need the following:

* CMake 3.12+,
* Python 2.7+ or 3.x,
* Git.

If you are going to be doing any development with OpenGL, then you will need
OpenGL 4.5+. Once all packages are installed, you can proceed depending on how
you plan to integrate Atlas into your project.

> **Note:**
> OpenGL cannot be downloaded as it is part of the driver for your graphics
> card. In order to find out which OpenGL version your GPU supports, please see
> manufacturer's website.

## Integrating Atlas

### Manually Adding the Source Code

At the top of your project directory, create a folder called `external` and
inside it create another folder called `atlas`. Inside this folder, copy the
source code for Atlas. Then, in your top-level `CMakeLists.txt` add the
following:

```cmake
add_subdirectory(${PROJECT_SOURCE_DIR}/external/atlas)
```

Then, once you have defined your project's target, add the following:

```cmake
target_link_libraries(your_target PUBLIC atlas::atlas)
```

Using `atlas::atlas` will link the entire library, but you can selectively link
modules if you don't need everything. The modules are:

* `atlas::math`,
* `atlas::glx`,
* `atlas::gui`,
* `atlas::hlr`,
* `atlas::utils`

Check the project's `README` for a list of the dependencies for each module.

An alternative version is to use a submodule in order to automatically download
the code and keep it up to date. To do this, from the root of your directory
execute the following:

```sh
git submodule add https://github.com/marovira/atlas.git external/atlas
git submodule init
git submodule update
```

This will add Atlas as a submodule located under `external/atlas`. The CMake
configuration remains the same.

### Automatically Adding Atlas Through CMake

A simpler approach involves using the `FetchContent` package from CMake in order
to automatically download and add the library to your project. This avoids
having to deal with submodules and maintaining a local copy of the code. To do
this, add the following in your top-level `CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
    atlas
    GIT_REPOSITORY https://github.com/marovira/atlas.git
    )
FetchContent_Populate(atlas)
add_subdirectory(${atlas_SOURCE_DIR} ${atlas_BINARY_DIR})
```

Once this is done, the necessary modules from Atlas can be linked as shown
above.

## Configuring Atlas

Atlas defines the following CMake options:

* `ATLAS_BUILD_TESTS`: this will build the unit tests shipped with Atlas and
  create the `RUN_TESTS` target.
* `ATLAS_BUILD_GL_TEST`: will build the OpenGL tests provided unit tests are
  being built.
* `ATLAS_BUILD_GUI_TEST`: will build the GUI tests provided unit tests are being
  built.

In order to ensure that your system is fully compatible, I recommend leaving
these options on and running the unit tests for Atlas on your first run. Once
the tests run successfully, you can turn `ATLAS_BUILD_TESTS` off and all unit
tests will be disabled. If you are not interested in the OpenGL/GUI side of
Atlas but still wish to run the unit tests, disable the corresponding options.

If you are running all unit tests, there are 4 tests that require user input.
These are:

* **Window Callback Test:** a black window will appear. Please make sure that
  you perform the following operations (not necessarily in this order):
    1. Click any mouse button inside the black area,
    2. Move the mouse within the black area,
    3. Scroll within the black area,
    4. Press any key (except for ESC) with the window in focus,
    5. Resize the window, and
    6. Close the window.
* **Multiple Windows:** two windows, one red and one green will appear. Click in
  both windows and then close them.
* **UI Tests:** there are two of these, but they both require the same action.
  Simply click the button that is mentioned in the UI.

If any of these tests fail, it is probably because an operation was not
captured. This is not entirely critical as it is more important that the windows
appear as it signals that GLFW was initialized correctly and that your system
supports the required version of OpenGL.

If all the tests passed, then your system fully supports Atlas and you are now
ready to begin developing your application.
