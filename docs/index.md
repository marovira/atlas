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

You can find out more using the links above to navigate through the
documentation. If you have any questions or suggestions on how to improve this,
please open a new issue.
