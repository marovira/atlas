# Frequently Asked Questions

## Why is OSX deprecated from Atlas?

The main reason is that Atlas targets OpenGL, and in the future I would like
to also use Vulkan. This creates a problem, since Apple announced in 2018 that
support for OpenGL would be removed and that all developers wanting to do
graphics on that platform should do so in Metal. Furthermore, Apple stopped
supporting OpenGL past version 4.2, which means that any code relying on modern
OpenGL designs will not work. Ideological opinions aside, I do not have an Apple
computer, nor do I have intentions of learning how to code in Metal and making
changes to Atlas to use it. As a result, I removed Apple support from the
framework.
