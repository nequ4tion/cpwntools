#include <cpwn.h>

int
main(int argc, char* const* argv)
{
  char* num_as_str = cstr.from_nstr("deadbeef");
  bn_t number = bn.from_cstr(num_as_str);
  cstr.dealloc(&num_as_str);
  num_as_str = bn.to_cstr(&number);
  printf("%s\n", num_as_str);
  cstr.dealloc(&num_as_str);
  return 0;
}