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

## What if I have macOS and want to use Atlas?

Support for macOS has been deprecated from Atlas. That being said, Atlas does
support Linux and Windows. As a result of this, you can work around the
deprecation by using a VM on OSX. There are several options available for this:

* Use [Boot Camp](https://support.apple.com/boot-camp) to install Windows along
  with macOS.
* Use a VM software like [Parallels Desktop](https://www.parallels.com/ca/),
  [VMWare Fusion](https://www.vmware.com/products/fusion.html), or
  [VirtualBox](https://www.virtualbox.org/).
* Using [Vagrant](https://www.vagrantup.com/) with
  [VirtualBox](https://www.virtualbox.org/)). If you go this route, please see
  some recommendations listed below.

> **Note:** If you use VM software, it is *very* likely that you will be unable to
> use the OpenGL utilities from Atlas as they require OpenGL 4.5+. To verify
> this, check the documentation for the software you are planning to use. In the
> case of Boot Camp, OpenGL support will be determined by the GPU installed on
> your computer. Please see the official specifications for your GPU to
> determine the version of OpenGL that it supports.

### Using Vagrant and VirtualBox

These are some guidelines for how to setup Vagrant and VirtualBox for
development with Atlas. Please note that these have not been tested extensively
and do not cover any troubleshooting. After installing both packages, open a
terminal and do the following:

```sh
# Setup Vagrant
vagrant init ubuntu/eoan64
vagrant up
vagrant ssh
cd /vagrant

# Optional updates
sudo apt update
sudo apt upgrade

# Install CMake
sudo apt install cmake

# Install OpenGL packages (required for compiling)
sudo apt install libgl1-mesa-dev
sudo apt install libxrandr-dev
sudo apt install libxinerama-dev
sudo apt install libxcursor-dev
sudo apt install libxi-dev

# Run CMake as normal.
```
