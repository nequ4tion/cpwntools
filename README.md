# cpwntools
This library tries to implement a C version of pwntools that is portable and easy to use.

# Usage

Functions are typically called by using provided namespaces like:

`char *string = cstr.from_nstr("Hello");`

For compilation of executables the flag `-lcpwn` is needed; if this library should be dynamically
linked, `-lcpwndyn` can be used.

Building shared libraries is done with

`make networking` for the networking library

`make stdpwn` for the standard library.

# Installation
The library can be installed on the system globally by invoking `make install`.
To remove it from the system `make deinstall` can be used.

# TODO

- [ ] Implement async networking
- [ ] Implement different protocols
- [ ] Implement android tools
- [ ] Implement serial communications
- [ ] Implement/Use a library with cryptographic primitives
- [ ] Better build system
- [x] Add bignum support
- [x] Add documentation for each module
- [ ] Write tests for all modules