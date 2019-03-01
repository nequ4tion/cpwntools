/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: bignum.h
 * PURPOSE: Provide an interface for a bignum backend.
 *************************************************************/

#ifndef BIGNUM_H
#define BIGNUM_H

#ifdef COMPILE_TIME
#include "../../../cpwn_extern/tiny-bignum-c/bn.h"
#else
#include <cpwn_extern/tiny-bignum-c/bn.h>
#endif /* COMPILE_TIME */

#include "boolean.h"
#include <stdint.h>

typedef struct
{
  struct bn inner;
} bn_t;

typedef enum
{
  Bigger,
  Smaller,
  Equal,
} equality_t;

struct bignum_h
{
  bn_t (*new)(void);
  bn_t (*from_int)(uint64_t integer);
  int (*to_int)(bn_t* bignumber);
  bn_t (*from_nstr)(char* string, int nbytes);
  bn_t (*from_cstr)(char* cstring);
  void (*to_nstr)(bn_t* bignumber, char* string, int stringlen);
  char* (*to_cstr)(bn_t* bignumber);

  /* Arithmetics */
  /* c = a + b */
  bn_t (*add)(bn_t* a, bn_t* b);
  /* c = a - b */
  bn_t (*sub)(bn_t* a, bn_t* b);
  /* c = a * b */
  bn_t (*mul)(bn_t* a, bn_t* b);
  /* c = a / b */
  bn_t (*div)(bn_t* a, bn_t* b);
  /* c = a % b */
  bn_t (*mod)(bn_t* a, bn_t* b);
  /* c = a/b, d = a%b */
  void (*divmod)(bn_t* a, bn_t* b, bn_t* c, bn_t* d);

  /* Bitwise operations */
  /* c = a & b */
  bn_t (*and)(bn_t* a, bn_t* b);
  /* c = a | b */
  bn_t (* or)(bn_t* a, bn_t* b);
  /* c = a ^ b */
  bn_t (*xor)(bn_t* a, bn_t* b);
  /* b = a << nbits */
  bn_t (*lshift)(bn_t* a, int nbits);
  /* b = a >> nbits */
  bn_t (*rshift)(bn_t* a, int nbits);

  /* Special operators and comparison */
  equality_t (*cmp)(bn_t* a, bn_t* b);
  bool_t (*is_zero)(bn_t* n);
  /* increment by 1 */
  void (*inc)(bn_t* n);
  /* decrement by 1 */
  void (*dec)(bn_t* n);
  /* c = a ^ b */
  bn_t (*pow)(bn_t* a, bn_t* b);
  /* integer square root b ∈ N ∪ {0} */
  bn_t (*isqrt)(bn_t* a);
  /* dest := src */
  void (*assign)(bn_t* dst, bn_t* src);
};

extern const struct bignum_h bn;

#endif /* BIGNUM_H */