# Atlas Framework v1.3

## What is Atlas?
It is a simple, light-weight graphics framework designed to support all types of OpenGL applications. Atlas is designed to be a teaching, prototyping, and research framework that allows users to quickly focus on solving graphics problems without having to worry about the details of how to set up things on their own machines. To this end, Atlas provides support for the following things:

* Cross-platform creation of windows and OpenGL contexts.
* Full vector, matrix, and quaternion library with support for all common graphics operations.
* Templated floating point comparison operators.
* Templated solvers for equations up to degree 4.
* Simple cross-platform logging system.
* Macros for identifying platforms as well as convenience operations such as unused parameters and debug code.
* A random number generator based on the new C++11 random library.
* Wrappers for shaders and textures.
* and more...

## Dependencies
For the most part, Atlas is a self-contained library. It does have two dependencies that are **not** included with Atlas. These are:

* GLEW. Used only in Linux and Windows. This is required for Atlas.
* CMake. Cross-platform build system employed by Atlas. This is required for all platforms.

Internally, Atlas ships with the following dependencies:

* GLFW. Used for window and context creation.
* GLM. Used for all math, vector, matrix, and quaternion operations.
* GLI. Used for loading textures.

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

## Future Plans, Features, etc.
The following are a few minor features that are planned for Atlas and that will be slowly added (in no particular order):

* Increased support for texture files. In particular, expansion to PPM, BMP, and PNG is planned.
* Wrapper for OpenGL framebuffer objects.
* Support for logging to a file.
* more to come...

The following major features are planned for Atlas:

* Support for multiple rendering contexts (may be coming soon!).
* Support for multi-threaded applications (tricky to implement).

## Contributing to Atlas.
At this point in time, you can support Atlas in one of two ways:

* Create a ticket in the repo requesting a bug/feature/enhancement.
* Making a pull request.

If you are planning on making a pull request, please make sure that you take note of the coding standards used throughout Atlas.

## Changelog
Please see [here](https://github.com/marovira/atlas/wiki/Changelog) for the list of new features/fixes!
