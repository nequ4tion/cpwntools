# cpwntools
This library tries to implement a C version of pwntools that is portable and easy to use.

# Usage

Functions are typically called by using provided namespaces like:

`char *string = cstr.from_cstr("Hello");`

For compilation of executables the flag `-lcpwn` is needed; if this library should be dynamically
linked, `-lcpwndyn` can be used.

Building shared libraries is done with

`make networking` for the networking library

`make stdpwn` for the standard library.

# TODO

- [ ] Implement async networking
- [ ] Implement different protocols
- [ ] Implement android tools
- [ ] Implement serial communications
- [ ] Implement/Use a library with cryptographic primitives
- [ ] Better build system
