# Atlas Framework 2.0

## What is Atlas?
It is a simple, light-weight graphics framework designed to support all types of
OpenGL applications. Atlas is designed to be a teaching, prototyping, and
research framework that allows users to quickly focus on solving graphics 
problems without having to worry about the details of how to set up things on 
their own machines.

## Internal Structre
Atlas is divided into a series of submodules. Below is a brief description of
each one. For full details, please see the documentation found 
[here](https://marovira.github.io/atlas/).

* ``core``: is the only independent module of Atlas, and contains a variety of
  features such as: float comparisons, useful constants, platform identifying
  macros, exception classes, timing system, header wrappers for external
  libraries.
* ``gl``: contains all the wrappers for OpenGL objects. These are: vertex array
  objects, buffer objects, textures, shaders, and an error callback.
* ``math``: contains typedefs for typically used types, along with polynomial
  solvers, coordinate switching functions, and a random number generator.
* ``utils``: contains a variety of utilities, including the Application class,
  which the core of any Atlas application, Geometries, amongst others.

## Dependencies
Atlas is a completely self-contained library that only requires CMake to build.
Internally, Atlas uses the following libraries:

* GLFW: Used for window and context creation.
* GL3W: Used to load OpenGL core profile functions.
* GLM: Used for all math, vector, matrix, and quaternion operations.
* TinyObj: Used for loading object files.
* STB Image: used to load image files for textures.
* ImGUI: Used to create simple GUIs.

## Quick Start with Atlas
The easiest way to use Atlas is by using CMake. In your project's CMakeLists file, add the following lines:

~~~{.CMake}
add_subdirectory(${ATLAS_DIR})
...
include_directories(${ATLAS_INCLUDE_DIRS})
...
target_link_libraries(${APP_NAME} ${ATLAS_LIBRARIES})
~~~

## Supported Platforms and Compilers
Atlas currently supports the following platforms:

* Windows (7/8/10)
* OSX (Yosemite)
* Linux

For compilers, please be aware that Atlas heavily uses C++11 features, as a result
the following compilers are supported:

* MSVC 12, 14 (ships with Visual Studio Community 2013 and 2015 respectively)
* GCC 4.3+
* LLVM Clang 3.3+
* Apple Clang 4.0+
* Intel C++ Composer XE 2013+ (experimental)

## Contributing to Atlas.
At this point in time, you can support Atlas in one of two ways:

* Create a ticket in the repo requesting a bug/feature/enhancement.
* Making a pull request.

If you are planning on making a pull request, please make sure that you take note of the coding standards used throughout Atlas.

## Changelog
Please see [here](https://github.com/marovira/atlas/wiki/Changelog) for the list of new features/fixes!
