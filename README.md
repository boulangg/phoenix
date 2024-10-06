[![Build + Unit Tests](https://github.com/boulangg/phoenix/actions/workflows/cmake.yml/badge.svg)](https://github.com/boulangg/phoenix/actions/workflows/cmake.yml)

# Phoenix

Compiling (and testing) this project required the foolowing tools:
* GCC 13.2.0 (or higher)
* GDB 15.0.50 (or higher)
* GRUB 2 (at least)
* libxorriso for creating the iso disk image (with grub-mkrescue).

To run the project:

	mkdir build && cd build
	cmake ..
	make launch

The project is configured to be built for Ubuntu 24.04 LTS on Windows WSL2.

# Resources

* [Memory mapping](doc/memory_map.md)

# Helpers

For VS 2022, the debug output is configured with the *.natvis files. To debug the natvis, you need to:
- View -> Other Windows -> Command Windows
- Debug.MIDebugLog /On /OutputWindow

You can take inspiration from %VS_Install_Folder%\Common7\IDE\CommonExtensions\Microsoft\Linux\Linux\stl.natvis
