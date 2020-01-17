# Atlas v3.0

> A graphics framework for education and prototyping.

[![Generic badge](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Generic badge](https://img.shields.io/badge/Language-C++17-red.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Build status](https://ci.appveyor.com/api/projects/status/anqkem3w8yq96wru?svg=true)](https://ci.appveyor.com/project/marovira/atlas)
[![Build Status](https://travis-ci.org/marovira/atlas.svg?branch=master)](https://travis-ci.org/marovira/atlas)

## What is Atlas?

Atlas is the result of my years learning graphics and GPU programming.
Initially, it started as a loose collection of wrappers for common OpenGL
operations, but eventually grew to be a full development framework. As it always
happens, throughout the journey, the internal specification for Atlas has
changed and evolved to match my current understanding of graphics and GPU
programming. This leads us to the current version, which abandons all pretenses
of wrappers in favour of a clean, concise API that should permit development of
*any* type of application.

It is important to understand that Atlas is **not** a rendering engine or a game
engine of any kind. It is a collection of modules that can be used to create an
engine or any other type of application while giving the user full control over
what is happening.

The key features of Atlas are as follows:

* Straight-forward, clear, function-based, data-driven API. In many ways, it
  takes inspiration from Vulkan's general structure. 
* Separation into independent modules makes adding them to your project easy.
  Just pick whichever modules you prefer and off you go.
* Convenience extensions to OpenGL and GLSL. Chief amongst them is a new file
  include system for GLSL source files that emulates as closely as possible the
  C++ inclusion system.

## Dependencies

The following are **core** requirements of Atlas:

* OpenGL 4.5+,
* CMake 3.12+,
* Python 2.7+ or 3.x.

Atlas supports the following platforms and compilers:

| Platform (Compiler) | Version |
| ------------------- | ------- |
| Windows (MSVC) | 19.23 |
| Linux (GCC) | 7.4.0 |
| Linux (LLVM Clang) | 7.0 |

Please note that macOS is **not** supported.

In addition, Atlas depends on the following libraries:

* [FMT](https://github.com/fmtlib/fmt),
* [GLM](https://github.com/g-truc/glm),
* [GLFW](https://github.com/glfw/glfw),
* [GL3W](https://github.com/marovira/gl3w),
* [ImGUI](https://github.com/ocornut/imgui),
* [STB](https://github.com/nothings/stb),
* [cr](https://github.com/fungos/cr).

The dependencies per module are as follows:

| Module | Dependencies |
| ------ | ------------ |
| core | FMT |
| math | GLM, core |
| glx | OpenGL, GLFW, gl3w, core |
| gui | glx, core, ImGUI (included) |
| hlr | core, cr (included) |
| utils | math, FMT, tinyobjloader, stb (included) | 

## Contributing

There are three ways in which you can contribute to Atlas:

* If you find a bug, please open an issue. Similarly, if you have a question
  about how to use it, or if something is unclear, please post an issue so it
  can be addressed.
* If you have a fix for a bug, or a code enhancement, please open a pull
  request. Before you submit it though, make sure to abide by the rules written
  below.
* If you have a feature proposal, you can either open an issue or create a pull
  request. If you are submitting a pull request, it must abide by the rules
  written below. Note that any new features need to be approved by me.

If you are submitting a pull request, the guidelines are the following:

1. Ensure that your code follows the standards and formatting of the framework.
   The coding standards can be seen throughout the code, and the formatting is
   handled through the `.clang-format` file located at the root of the
   directory. Any changes that do not follow the style and format will be
   rejected.
2. Ensure that *all* unit tests are working prior to submitting the pull
   request. If you are adding a new feature that has been approved, it is your
   responsibility to provide the corresponding unit tests (if applicable). 

## License

Atlas is published under the MIT license and can be viewed
[here](https://github.com/marovira/atlas/blob/master/LICENSE). Atlas also
bundles third-party code that is distributed under their corresponding licenses
which can be seen
[here](https://github.com/marovira/atlas/blob/master/LICENSE-3RD-PARTY).

## Documentation

The documentation for Atlas is currently being re-written. Currently, Atlas
ships with some ready-made examples of simple cases contained under the
`examples` folder. For more help, including tutorials, instructions, etc. see
[here](https://marovira.github.io/atlas/).
