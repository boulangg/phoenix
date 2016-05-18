# Phoenix

Compiling (and testing) this project required the foolowing tools:
* GCC 4.8.4 (or higher)
* GDB 7.9.1 (or higher)
* GRUB 2 (at least)
* libxorriso for creating the iso disk image (with grub-mkrescue).

The make debug target require a patch on gdb:
https://sourceware.org/bugzilla/attachment.cgi?id=8512&action=diff
 (depending on the GDB version the line numbers might not correspond). This
 patch allows GDB to be able to swith architecture (from x86 to x64) at
 runtime.

# Resources

* [Memory mapping](doc/memory_map.md)
