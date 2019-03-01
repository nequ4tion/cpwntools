/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: bignum.c
 * PURPOSE: Provide an interface for a bignum backend.
 *************************************************************/

#include "bignum.h"
#include "../../../cpwn_extern/tiny-bignum-c/bn.h"
#include "cstring.h"
#include <stdint.h>

static bn_t
bn_new(void)
{
  bn_t ret = { { { 0 } } };
  bignum_init(&ret.inner);
  return ret;
}

static bn_t
bn_from_int(uint64_t integer)
{
  bn_t ret = { { { 0 } } };
  bignum_from_int(&ret.inner, integer);
  return ret;
}

static int
bn_to_int(bn_t* bignumber)
{
  return bignum_to_int(&bignumber->inner);
}

static bn_t
bn_from_nstr(char* string, int nbytes)
{
  bn_t ret = { { { 0 } } };
  bignum_from_string(&ret.inner, string, nbytes);
  return ret;
}

static bn_t
bn_from_cstr(char* cstring)
{
  bn_t ret = { { { 0 } } };
  bignum_from_string(&ret.inner, cstring, cstr.strlen(cstring));
  return ret;
}

static void
bn_to_nstr(bn_t* bignumber, char* string, int stringlen)
{
  bignum_to_string(&bignumber->inner, string, stringlen);
}

static char*
bn_to_cstr(bn_t* bignumber)
{
  /* tiny-bignum-c has 256 as its longest length for a hexadecimal string */
  size_t max_bn_hex_len = 256;

  /* adding +1 for trailing NUL-byte and another +1 to make the number even */
  char* ret = cstr.new_str(max_bn_hex_len + 2);
  cstr.fill_bytes(ret, '\0', max_bn_hex_len + 2);
  bignum_to_string(&bignumber->inner, ret, cstr.strlen(ret));
  return ret;
}

/* result = bn_add(a, b) is the same as result := a + b */
static bn_t
bn_add(bn_t* a, bn_t* b)
{
  bn_t c = bn_new();
  /* c = a + b */
  bignum_add(&a->inner, &b->inner, &c.inner);
  return c;
}

/* result = bn_sub(a, b) is the same as result := a - b */
static bn_t
bn_sub(bn_t* a, bn_t* b)
{
  bn_t c = bn_new();
  /* c = a - b */
  bignum_sub(&a->inner, &b->inner, &c.inner);
  return c;
}

/* result = bn_mul(a, b) is the same as result := a * b */
static bn_t
bn_mul(bn_t* a, bn_t* b)
{
  bn_t c = bn.new();
  /* c = a * b */
  bignum_mul(&a->inner, &b->inner, &c.inner);
  return c;
}

/* result = bn_div(a, b) is the same as result := a / b */
static bn_t
bn_div(bn_t* a, bn_t* b)
{
  bn_t c = bn.new();
  /* c = a / b */
  bignum_div(&a->inner, &b->inner, &c.inner);
  return c;
}

/* result = bn_mod(a, b) is the same as result := a mod b */
static bn_t
bn_mod(bn_t* a, bn_t* b)
{
  bn_t c = bn.new();
  /* c = a % b */
  bignum_mod(&a->inner, &b->inner, &c.inner);
  return c;
}

/* c = a/b, d = a%b */
static void
bn_divmod(bn_t* a, bn_t* b, bn_t* c, bn_t* d)
{
  bignum_divmod(&a->inner, &b->inner, &c->inner, &d->inner);
}

/* result = bn_and(a, b) is the same as result := a & b */
static bn_t
bn_and(bn_t* a, bn_t* b)
{
  bn_t c = bn.new();
  /* c = a & b */
  bignum_and(&a->inner, &b->inner, &c.inner);
  return c;
}

/* result = bn_or(a, b) is the same as result := a | b */
static bn_t
bn_or(bn_t* a, bn_t* b)
{
  bn_t c = bn.new();
  /* c = a | b */
  bignum_or(&a->inner, &b->inner, &c.inner);
  return c;
}

/* result = bn_xor(a, b) is the same as result := a ^ b */
static bn_t
bn_xor(bn_t* a, bn_t* b)
{
  bn_t c = bn.new();
  /* c = a ^ b */
  bignum_xor(&a->inner, &b->inner, &c.inner);
  return c;
}

/* result = bn_lshift(a, nbits) is the same as result := a << b */
static bn_t
bn_lshift(bn_t* a, int nbits)
{
  bn_t b = bn.new();
  /* b = a << nbits */
  bignum_lshift(&a->inner, &b.inner, nbits);
  return b;
}

/* result = bn_rshift(a, nbits) is the same as result := a >> b */
static bn_t
bn_rshift(bn_t* a, int nbits)
{
  bn_t b = bn.new();
  /* b = a >> nbits */
  bignum_rshift(&a->inner, &b.inner, nbits);
  return b;
}

equality_t
bn_cmp(bn_t* a, bn_t* b)
{
  int bn_cmp_result = bignum_cmp(&a->inner, &b->inner);
  switch (bn_cmp_result) {
    case LARGER:
      return Bigger;
      break;
    case SMALLER:
      return Smaller;
      break;
    case EQUAL:
      return Equal;
      break;
    default:
      /* memory corruption detected */
      PANIC("bn_cmp", __FILE__, __LINE__);
  }
}

bool_t
bn_is_zero(bn_t* n)
{
  if (bignum_is_zero(&n->inner)) {
    return true;
  } else {
    return false;
  }
}

/* increment by 1 */
static void
bn_inc(bn_t* n)
{
  bignum_inc(&n->inner);
}

/* decrement by 1 */
static void
bn_dec(bn_t* n)
{
  bignum_dec(&n->inner);
}

/* result = bn_pow(a, nbits) is the same as result := a ** b */
static bn_t
bn_pow(bn_t* a, bn_t* b)
{
  bn_t c = bn.new();
  /* c = a ** b */
  bignum_pow(&a->inner, &b->inner, &c.inner);
  return c;
}

/* integer square root */
static bn_t
bn_isqrt(bn_t* a)
{
  bn_t b = bn.new();
  bignum_isqrt(&a->inner, &b.inner);
  return b;
}

/* dest := src */
static void
bn_assign(bn_t* dest, bn_t* src)
{
  bignum_assign(&dest->inner, &src->inner);
}

const struct bignum_h bn = { .new = bn_new,
                             .from_int = bn_from_int,
                             .to_int = bn_to_int,
                             .from_nstr = bn_from_nstr,
                             .from_cstr = bn_from_cstr,
                             .to_nstr = bn_to_nstr,
                             .to_cstr = bn_to_cstr,
                             .add = bn_add,
                             .sub = bn_sub,
                             .mul = bn_mul,
                             .div = bn_div,
                             .mod = bn_mod,
                             .divmod = bn_divmod,
                             .and = bn_and,
                             .or = bn_or,
                             .xor = bn_xor,
                             .lshift = bn_lshift,
                             .rshift = bn_rshift,
                             .cmp = bn_cmp,
                             .is_zero = bn_is_zero,
                             .inc = bn_inc,
                             .dec = bn_dec,
                             .pow = bn_pow,
                             .isqrt = bn_isqrt,
                             .assign = bn_assign };
