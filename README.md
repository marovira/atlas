# Atlas Framework

## What is the Atlas Framework?
It is a simple, light-weight graphics framework designed to support all types of graphics applications (whether they use OpenGL or not). It currently supports the following things:

* Cross-platform ceation of windows with OpenGL contexts (through GLFW).
* Full vector and matrix library with support for all common graphics operations (through GLM).
* Templated floating point comparisons.
* Templated solvers for polynomials of degrees 2 through 4.
* Simple cross-platform logging system.
* Macros for identifying platforms as well as convenience operations such as unused parameters and debug code.
* A random number generator based on the new C++11 random library.
* A simple wrapper for all OpenGL shader operations.

## Dependencies
Atlas uses CMake to build and GLEW for OpenGL functions in Windows and Linux. GLM and GLFW are already packaged in the `external/` directory.

## How to use it
The easiest way to use Atlas is to add the following lines to your CMakeLists file:

```CMake
add_subdirectory(${ATLAS_DIR})
...
include_directories(${ATLAS_INCLUDE_ROOT} ...)
...
target_link_libraries(${APP_NAME} atlas ...)
```
More ways will be tested soon.

## Supported platforms
Atlas currently supports the following platforms:

* Windows (7/8/10)
* OSX
* Linux

## Planned Features
Currently, the two big features that will be added in the near future are:

1. Support for multiple windows with multiple rendering contexts.
2. Support for multi-threaded applications.

If you have any requests, please open a ticket in the repository.
