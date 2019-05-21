#include <cpwn.h>

int
main(int argc, char* const* argv)
{
  /* must be provided as hexadecimal; 0xdeadbeef = 3735928559 */
  char* num_as_str = cstr.from_nstr("deadbeef");

  /* bn works with cstr */
  bn_t number = bn.from_cstr(num_as_str);

  /* the string is no longer needed */
  cstr.dealloc(&num_as_str);

  /* works the other way around as well */
  num_as_str = bn.to_cstr(&number);

  printf("%s\n", num_as_str);
  
  /* deallocate to avoid memory leaks, bn uses a library with no dynamic allocations */
  cstr.dealloc(&num_as_str);

  return 0;
}