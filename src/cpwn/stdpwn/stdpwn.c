#include <stdint.h>
#include <stdio.h>

void
stdpwn_printhex_u8(uint8_t x)
{
  fprintf(stdout, "%hhx\n", x);
}

void
stdpwn_printhex_u16(uint16_t x)
{
  fprintf(stdout, "%hx\n", x);
}

void
stdpwn_printhex_u32(uint32_t x)
{
  fprintf(stdout, "%x\n", x);
}

void
stdpwn_printhex_u64(uint64_t x)
{
  fprintf(stdout, "%lx\n", x);
}

void
stdpwn_printhex_i8(int8_t x)
{
  fprintf(stdout,
          "%s%hhx\n",
          x < 0 ? "-" : "",
          x < 0 ? (uint8_t)((-1) * x) : (uint8_t)x);
}

void
stdpwn_printhex_i16(int16_t x)
{
  fprintf(stdout,
          "%s%hx",
          x < 0 ? "-" : "",
          x < 0 ? (uint16_t)((-1) * x) : (uint16_t)x);
}

void
stdpwn_printhex_i32(int32_t x)
{
  fprintf(stdout,
          "%s%x",
          x < 0 ? "-" : "",
          x < 0 ? (uint32_t)((-1) * x) : (uint32_t)x);
}

void
stdpwn_printhex_i64(int64_t x)
{
  fprintf(stdout,
          "%s%lx",
          x < 0 ? "-" : "",
          x < 0 ? (uint64_t)((-1) * x) : (uint64_t)x);
}

void
stdpwn_print_u8(uint8_t x)
{
  fprintf(stdout, "%hhu\n", x);
}

void
stdpwn_print_u16(uint16_t x)
{
  fprintf(stdout, "%hu\n", x);
}

void
stdpwn_print_u32(uint32_t x)
{
  fprintf(stdout, "%u\n", x);
}

void
stdpwn_print_u64(uint64_t x)
{
  fprintf(stdout, "%lu\n", x);
}

void
stdpwn_print_i8(int8_t x)
{
  fprintf(stdout, "%hhd\n", x);
}

void
stdpwn_print_i16(int16_t x)
{
  fprintf(stdout, "%hd\n", x);
}

void
stdpwn_print_i32(int32_t x)
{
  fprintf(stdout, "%d", x);
}

void
stdpwn_print_i64(int64_t x)
{
  fprintf(stdout, "%ld", x);
}
