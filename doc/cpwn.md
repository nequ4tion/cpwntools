* [cpwn](#cpwn)
  * [stdpwn](#stdpwn)
    * [alloc](#alloc)
    * [archdefs](#archdefs)
    * [bn (bignum)](#bn-bignum)
    * [colors](#colors)
    * [cstr (cstring)](#cstr-cstring)
    * [error](#error)
    * [stdtypes](#stdtypes)
  * [networking](#networking)
    * [tcp](#tcp)

# cpwn
## stdpwn
### alloc
The `alloc` module consists of a set of functions that allocate or deallocate dynamic memory.

#### malloc 
```C
void * alloc.malloc(size_t size)
```
The `alloc.malloc` function allocates a chunk of memory that is equal or larger than `size` bytes. Works similar to `stdlib.h`'s `malloc` function.
##### Remarks
This function [panics](####-PANIC) whenever the underlying `malloc` fails and returns `NULL`.

#### zero_alloc
```C
void * alloc.zero_alloc(size_t size)
```
The `alloc.zero_alloc` function allocates `size` bytes of memory. It then initializes the allocated memory with zero.
##### Remarks
This function [panics](####-PANIC) whenever the underlying `calloc` fails and returns `NULL`.

#### zero_allocarray
```C
void * alloc.zero_allocarray(size_t nmemb, size_t size)
```
The `alloc.zero_allocarray` function allocates `nmemb` elements of `size` bytes each. The allocated memory will be initialized with 0.
##### Remarks
This function [panics](####-PANIC) whenever the underlying `calloc` fails and returns `NULL`.

#### realloc
```C
void * alloc.realloc(void *ptr, size_t size)
```
The `alloc.realloc` function reallocates the memory that `ptr` points to and returns the newly allocated chunk. In practice this means, that it, when needed, frees the memory, allocates memory larger than or equal to `size` bytes and returns the result of that allocation.
##### Remarks
This function [panics](####-PANIC) whenever the underlying `realloc` fails and returns `NULL`.

#### free
```C
void alloc.free(void *ptr)
```
The `alloc.free` function deallocates the allocated memory that `ptr` points to.

#### sfree
```C
void alloc.sfree(void **ptr)
```
The `alloc.sfree` function takes an argument which is a pointer to a pointer that points to dynamically allocated memory, and similarly to [alloc.free](####free), frees the memory. After freeing the memory the original pointer is modified to be `NULL`.
##### Example
```C
void * ptr = alloc.malloc(5);
alloc.sfree(&ptr);
assert(ptr == NULL);
```

### archdefs
This file contains the following definitions to be able to distinguish on which platform and architecture the code is compiled.

* X86_64_ARCH: Set to 1 if on x86_64
* X86_ARCH:  Set to 1 if on x86
* IA64_ARCH: Set to 1 if on IA64
* PPC_ARCH: Set to 1 if on PPC
* PPC64_ARCH: Set to 1 if on PPC64
* SPARC_ARCH: Set to 1 if on SPARC
* UNDEFINED_ENDIAN: Set to 1 if no clear endianess
* PROCESSOR_LITTLE_ENDIAN: Set to 1 if on little endian
* PROCESSOR_BIG_ENDIAN: Set to 1 if big endian

### bn (bignum)
This module is a wrapper for [this](https://github.com/kokke/tiny-bignum-c) library.
#### new
```C
bn_t bn.new(void)
```
The `bn.new` function constructs a new `bn_t` bignum struct and sets it to zero.

#### from_int
```C
bn_t bn.from_int(uint64_t integer)
```
The `bn.from_int` function constructs a new `bn_t` bignum struct from a 64-bit integer.

#### from_nstr
```C
bn_t bn.from_nstr(char *string, int nbytes)
```
The `bn.from_nstr` function constructs a new `bn_t` bignum struct from a `string` of `nbytes` length.
##### Remarks
* The `string` should be a hexadecimal representation of a number.
* It needs to consist of an even number of characters. Leading 0s are allowed.
* The length of the `string` needs to be larger or equal to 8 characters. For smaller numbers the use of [bn.from_int](####-from_int) is recommended.

#### from_cstr
```C
bn_t bn.from_cstr(char *cstring)
```
The `bn.from_cstr` function constructs a new `bn_t` bignum struct from a `cstring` that has been constructed by the [`cstr`](###-cstr-(cstring)) module.
##### Remarks
* The `cstring` should be a hexadecimal representation of a number.
* It needs to consist of an even number of characters. Leading 0s are allowed.
* The length of the `cstring` needs to be larger or equal to 8 characters. For smaller numbers the use of [bn.from_int](####-from_int) is recommended.

#### to_int
```C
int bn.to_int(bn_t *bignumber)
```
Converts a `bignumber` to an integer.
##### Remarks
There might be a signed integer overflow if the `bignumber` is larger than the maximum value of the system's `int` type, which is typically an `int32_t`.

#### to_nstr
```C
void bn.to_nstr(bn_t *bignumber, char *string, size_t stringlen)
```
Converts a `bignumber` to its hexadecimal string representation. The result will be written to `string` and will contain at most `stringlen` bytes.
##### Remarks
The string length may be at most 256 bytes, this constraint is given by the tiny-bignum-c library, that is used as the underlying bignum library.

#### to_cstr
```C
char *bn.to_cstr(bn_t *bignumber)
```
Converts a `bignumber` to its hexadecimal representation and returns it as a cstring from the [`cstr`](###-cstr-(cstring)) module.

#### add
```C
bn_t bn.add(bn_t *a, bn_t *b)
```
Adds `a` and `b` and returns the result.

#### sub
```C
bn_t bn.sub(bn_t *a, bn_t *b)
```
Substracts `b` from `a` (`a-b`) and returns the result.

#### mul
```C
bn_t bn.mul(bn_t *a, bn_t *b)
```
Multiplies `a` with `b` and returns the result.

#### div
```C
bn_t bn.div(bn_t *a, bn_t *b)
```
Divides `a` by `b` (`a/b`) and returns the result.

#### mod
```C
bn_t bn.mod(bn_t *a, bn_t *b)
```
Calculates the modulus of `a` and `b` (`a (mod b)`) and returns the result.

#### divmod
```C
void bn.divmod(bn_t *a, bn_t *b, bn_t *c, bn_t *d)
```
Calculates `c = a/b` and `d = a (mod b)`.

#### and
```C
bn_t bn.and(bn_t *a, bn_t *b)
```
Applies the binary and (&) operation (`a&b`) and returns the result.

#### or
```C
bn_t bn.or(bn_t *a, bn_t *b)
```
Applies the binary or (|) operation (`a|b`) and returns the result.

#### lshift
```C
bn_t bn.lshift(bn_t *a, int nbits)
```
Applies the binary left bitshift operation (<<) (`a << nbits`) and returns the result.

#### rshift
```C
bn_t bn.rshift(bn_t *a, int nbits)
```
Applies the binary right bitshift operation (>>) (`a >> nbits`) and returns the result.

#### cmp
```C
equality_t bn.cmp(bn_t *a, bn_t *b)
```
Compares `a` and `b` and returns the result.
The result is an enum defined as the type `equality_t` that contains the members `Bigger`, `Smaller` and `Equal`.

#### is_zero
```C
bool_t bn.is_zero(bn_t *n)
```
Returns `true` if `n` is zero and `false` if it does not equal zero.

#### inc
```C
void bn.inc(bn_t *n)
```
Increments `n` by 1 and stores the result in `n`.

#### dec
```C
void bn.dec(bn_t *n)
```
Decrements `n` by 1 and stores the result in `n`.

#### isqrt
```C
bn_t bn.isqrt(bn_t *a)
```
Calculates the integer square root of `a` and returns the result.
For the result of the integer square root $b$, that this function implements, the statement $b \in \mathbb{Z}^{+}_{0}$ holds to be true.

#### assign
```C
void bn.assign(bn_t *dst, bn_t *src)
```
Assigns `src` to `dst`.

### colors
The colors module provides constants to make terminal output colorful or have a different style.
Self-explanatory names will not be explained in the following.

#### styles
* `colors.styles.reset`: Resets the style to default
* `colors.styles.bold`
* `colors.styles.underline`
* `colors.styles.inverse`: Inverts the colors

#### normal_fg_colors
Sets the foreground colors.
* `colors.normal_fg_colors.black`
* `colors.normal_fg_colors.red`
* `colors.normal_fg_colors.green`
* `colors.normal_fg_colors.yellow`
* `colors.normal_fg_colors.blue`
* `colors.normal_fg_colors.magenta`
* `colors.normal_fg_colors.cyan`
* `colors.normal_fg_colors.white`

#### normal_bg_colors
Sets the background colors.
* `colors.normal_bg_colors.black`
* `colors.normal_bg_colors.red`
* `colors.normal_bg_colors.green`
* `colors.normal_bg_colors.yellow`
* `colors.normal_bg_colors.blue`
* `colors.normal_bg_colors.magenta`
* `colors.normal_bg_colors.cyan`
* `colors.normal_bg_colors.white`

#### strong_fg_colors
Sets strong foreground colors.
* `colors.strong_fg_colors.black`
* `colors.strong_fg_colors.red`
* `colors.strong_fg_colors.green`
* `colors.strong_fg_colors.yellow`
* `colors.strong_fg_colors.blue`
* `colors.strong_fg_colors.magenta`
* `colors.strong_fg_colors.cyan`
* `colors.strong_fg_colors.white`

#### strong_bg_colors
Sets strong background colors.
* `colors.strong_bg_colors.black`
* `colors.strong_bg_colors.red`
* `colors.strong_bg_colors.green`
* `colors.strong_bg_colors.yellow`
* `colors.strong_bg_colors.blue`
* `colors.strong_bg_colors.magenta`
* `colors.strong_bg_colors.cyan`
* `colors.strong_bg_colors.white`

### cstr (cstring)
Implements non-nul-terminated strings in C.

#### from_nstr
```C
char *cstr.from_nstr(const char *str)
```
Creates a new cstring from a NUL-terminated C-style string.
##### Remarks
Do NOT try to deallocate the returned string with `free`. Use the [`cstr.dealloc`](####-dealloc) function for that.

#### new_str
```C
char *cstr.new_str(size_t capacity)
```
Creates a new cstring with the specified capacity and returns it.

##### Remarks
Do NOT try to deallocate the returned string with `free`. Use the [`cstr.dealloc`](####-dealloc) function for that.

#### from_buf
```C
char *cstr.from_buf(const char* buf, size_t buflen)
```
Creates a new cstring from `buf` with the specified `buflen`. NUL-bytes will be ignored.

##### Remarks
Do NOT try to deallocate the returned string with `free`. Use the [`cstr.dealloc`](####-dealloc) function for that.

#### strlen
```C
char *cstr.strlen(const char* str)
```
Returns the length of a cstring. (NOT ITS CAPACITY!!!)

#### buftostr
```C
char *cstr.buftostr(char* str, const char * buf, size_t buflen)
```
Copies a buf to an already allocated cstring. This lets your reuse cstrings.

##### Remarks
Do not use the argument `str` after the use of this function. Use the returned pointer instead, or better yet assign the argument to the return value of this function.

#### dealloc
```C
void cstr.dealloc(char** str)
```
Deallocates a string. No other function should be used to do that. `str` is pointer to a pointer that points to a cstring and will be set to `NULL` upon completion of the subroutine.

#### resize
```C
char *cstr.resize(char* str, size_t newsize)
```
Adjusts the capacity of a cstring `str` to be `newsize`.

##### Remarks
Do not use the argument `str` after the use of this function. Use the returned pointer instead, or better yet assign the argument to the return value of this function.

#### append_data
```C
void cstr.append_data(char** str, const char* buf, size_t buflen)
```
Appends a buffer of specified length to the string.

#### append
```C
void cstr.append(char** str, const char* otherstr)
```
Appends `otherstr` to `str`.

#### strdup
```C
char *cstr.strdup(const char* str)
```
Duplicates `str` and returns the duplicate.

#### fprint
```C
void cstr.fprint(FILE* fileptr, const char* str)
```
Writes `str` to the `fileptr` I/O stream.

#### fill_bytes
```C
void cstr.fill_bytes(char* str, char byte, size_t len)
```
Fills `str` with `len` bytes of kind `byte`.

#### capacity
```C
size_t cstr.capacity(char *str)
```
Returns the capacity of the given string. NOT its length.

### error
Implements error handling.

#### PANIC
```C
__attribute__((__noreturn__)) void
PANIC(const char* functionname,
      const char* filename,
      const unsigned long long linenum)
```
Panics with an error message that contains the function name, the file name and the line number.

### stdtypes
Defines types to make them convenient to use.

* `uint8_t`  as `u8`
* `uint16_t` as `u16`
* `uint32_t` as `u32`
* `uint64_t` as `u64`
* `int8_t`   as `i8`
* `int16_t`  as `i16`
* `int32_t`  as `i32`
* `int64_t`  as `i64`

If the GNU compiler is used, one can make use use of the 128-bit integer emulation.
This type is defined as `uint128_t` and `int128_t`.

Architecture specific types are aliased like:
* `size_t` as `sz_t`
* `ssize_t` as `ssz_t`

## networking
### tcp
Implements an easily usable TCP/IP library.

#### remote
```C
sock_t tcp.remote(const char *hostname, const port_t port)
```
Connects to a remote host with the specified `hostname` and `port` and returns the handle.
##### Remarks
Port is a 16-bit unsigned integer.

#### send
```C
ssz_t tcp.send(sock_t *sock, const char *buf, const sz_t buflen)
```
Uses the `sock` socket handle to send `buflen` bytes of `buf` and returns how many bytes were sent. If the return value is -1, then an error has occurred and `errno` is set appropriately.

#### recv
```C
ssz_t tcp.recv(sock_t *sock, char *buf, const sz_t buflen)
```
Uses the `sock` handle to receive `buflen` bytes, which will be written in `buf` and returns how many bytes have actually been received. If the return value is -1, then an error has occurred and `errno` is set appropriately.

#### close
```C
void tcp.close(sock_t *sock)
```
Closes the specified socket.

#### shutdown
```C
void tcp.shutdown(sock_t *sock)
```
Sends a signal to shut down the connection on both ends (read and write).

#### server
```C
serv_sock_t tcp.server(const char *hostname, const port_t port)
```
Binds a TCP server socket to the specified port and returns a handle to it.

#### valid
```C
bool_t tcp.valid(const sock_t sock)
```
Returns `true` if `sock` is a valid socket and `false` if `sock` is not a valid socket.
