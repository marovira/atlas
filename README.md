# Atlas (dev)

> A data-driven graphics development framework.

[![Generic badge](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Generic badge](https://img.shields.io/badge/Language-C++17-red.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)

## What is Atlas?

Atlas is the result of my years learning graphics and GPU programming.
Initially, it started as a loose collection of wrappers for common OpenGL
operations, but eventually grew to be a full development framework. As it always
happens, throughout the journey, the internal specification for Atlas has
changed and evolved to match my current understanding of graphics and GPU
programming. This leads us to the current version, which abandons all pretenses
of wrappers in favour of a clean, concise API that should permit development of
*any* type of application.

The key features of Atlas are as follows:

* Straight-forward, clear, function-based, data-driven API. In many ways, it
  takes inspiration from Vulkan's general structure. 
* Separation into independent modules makes adding them to your project easy.
  Just pick whichever modules you prefer and off you go.
* Convenience extensions to OpenGL and GLSL. Chief amongst them is a new file
  include system for GLSL source files that emulates as closely as possible the
  C++ inclusion system.
* Live-library reloading. Allows for a significantly faster and more streamlined
  workflow that does not waste time re-compiling the entire application every
  time a change needs to be made.

## Dependencies

The following are **core** requirements of Atlas:

* OpenGL 4.5+,
* CMake 3.12+.

Atlas supports the following C++17 compliant compilers:

| Compiler | Version |
| -------- | ------- |
| MSVC | 16.1 |
| GCC | 9.1 |
| LLVM Clang | 8.0 |

Please note that OSX is **not** supported.

In addition, Atlas depends on the following libraries:

* [FMT](https://github.com/fmtlib/fmt),
* [GLM](https://github.com/g-truc/glm),
* [GLFW](https://github.com/glfw/glfw),
* [GL3W](https://github.com/marovira/gl3w),
* [ImGUI](https://github.com/ocornut/imgui),
* [STB](https://github.com/nothings/stb).

The dependencies per module are as follows:

| Module | Dependencies |
| ------ | ------------ |
| core | FMT |
| math | GLM, core |
| glx | OpenGL, GLFW, gl3w, core |
| gui | glx, core, ImGUI (included) |
| hlr | core |
| utils | math, FMT, tinyobjloader, stb (included) | 

## Contributing

For the time-being, pull requests are not accepted. This is due to the fact that
Atlas is undergoing a major redesign. I will change this note when a more stable
state has been attained.
