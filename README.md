# cpwntools
<img src="cpwnlogo.png" alt="drawing" width="200" heigt="200"/>

This library implements a C version of the pwntools library for writing kernel exploits and
creating exploits that require a high performance.

# Usage

Functions are typically called by using provided namespaces like:

`char *string = cstr.from_nstr("Hello");`

For compilation of executables the flag `-lcpwn` is needed; if this library should be dynamically
linked, `-lcpwndyn` can be used. This only works when the library has been installed globally.

Documentation on all implemented modules can be built using Doxygen. 
This can be done by invoking `./manage doc` in the projects root directory.
To view it one can use `./manage opendocs` which will start a python server bound to localhost on the port 5000. A webbrowser should open automatically.
The python server can be killed by invoking `./manage killdocserver`.

# Installation
The library can be installed on the system globally by invoking `./manage install`.
To remove it from the system `./manage uninstall` can be used.

# Working systems
This library has been tested on Arch Linux, Microsoft Windows 10 and OpenBSD, but might work on other
systems as well.
