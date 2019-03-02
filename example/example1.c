#include <cpwn.h>

#include <stdio.h>
int
main (int argc, char * const *argv)
{
  char *data = cstr.from_nstr ("Hello");
  sock_t s = tcp.remote ("127.0.0.1", 1337);
  printf ("%ld\n", tcp.send (&s, data, 5));
  tcp.shutdown(&s);
  tcp.close (&s);
  cstr.dealloc (&data);
  return 0;
}
