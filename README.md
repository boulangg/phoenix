[![Build + Unit Tests](https://github.com/boulangg/phoenix/actions/workflows/cmake.yml/badge.svg)](https://github.com/boulangg/phoenix/actions/workflows/cmake.yml)

# Phoenix

Compiling (and testing) this project required the foolowing tools:
* GCC 11.2.0 (or higher)
* GDB 12.1 (or higher)

## Build and Run

### Visual Studio with WSL

This project can be build and tested from Visual Studio on Windows 11 usng WSL2.

- Install Visual Studio 2022.
- Install Ubuntu 22.04 LTS in WSL2.
	- Verify that the version of GDB is at least 12.1 (there is a bug in 12.0.98)
- Open the folder as a CMake project in Visual Studio
- Select `WSL: Ubuntu-22.04 as the running machine`
- Select `Build (linux-default-build-default)` as build configuration

- For unit tests, select `libunittestsuite.so` in the run target and run it. It will trigger the debugger automatically.

- You can't run the kernel directly from Visual Studio (yet)

### Ubunutu

To run the project:

	mkdir build && cd build
	cmake ..
	make launch

# Resources

* [Memory mapping](doc/memory_map.md)
