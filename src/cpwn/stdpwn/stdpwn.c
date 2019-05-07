#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

void
stdpwn_printhex_u8(uint8_t x)
{
  fprintf(stdout, "%" PRIx8 "\n", x);
}

void
stdpwn_printhex_u16(uint16_t x)
{
  fprintf(stdout, "%" PRIx16 "\n", x);
}

void
stdpwn_printhex_u32(uint32_t x)
{
  fprintf(stdout, "%" PRIx32 "\n", x);
}

void
stdpwn_printhex_u64(uint64_t x)
{
  fprintf(stdout, "%" PRIx64 "\n", x);
}

void
stdpwn_printhex_i8(int8_t x)
{
  fprintf(stdout,
          "%s%" PRIx8 "\n",
          x < 0 ? "-" : "",
          x < 0 ? (uint8_t)((-1) * x) : (uint8_t)x);
}

void
stdpwn_printhex_i16(int16_t x)
{
  fprintf(stdout,
          "%s%" PRIx16 "\n",
          x < 0 ? "-" : "",
          x < 0 ? (uint16_t)((-1) * x) : (uint16_t)x);
}

void
stdpwn_printhex_i32(int32_t x)
{
  fprintf(stdout,
          "%s%" PRIx32 "\n",
          x < 0 ? "-" : "",
          x < 0 ? (uint32_t)((-1) * x) : (uint32_t)x);
}

void
stdpwn_printhex_i64(int64_t x)
{
  fprintf(stdout,
          "%s%" PRIx64 "\n",
          x < 0 ? "-" : "",
          x < 0 ? (uint64_t)((-1) * x) : (uint64_t)x);
}

void
stdpwn_print_u8(uint8_t x)
{
  fprintf(stdout, "%" PRIu8 "\n", x);
}

void
stdpwn_print_u16(uint16_t x)
{
  fprintf(stdout, "%" PRIu16 "\n", x);
}

void
stdpwn_print_u32(uint32_t x)
{
  fprintf(stdout, "%" PRIu32 "\n", x);
}

void
stdpwn_print_u64(uint64_t x)
{
  fprintf(stdout, "%" PRIu64 "\n", x);
}

void
stdpwn_print_i8(int8_t x)
{
  fprintf(stdout, "%" PRIi8 "\n", x);
}

void
stdpwn_print_i16(int16_t x)
{
  fprintf(stdout, "%" PRIi16 "\n", x);
}

void
stdpwn_print_i32(int32_t x)
{
  fprintf(stdout, "%" PRIi32 "\n", x);
}

void
stdpwn_print_i64(int64_t x)
{
  fprintf(stdout, "%" PRIi64 "\n", x);
}
