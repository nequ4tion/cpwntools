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
  /**
   * @brief Inner bn struct to interact with the
   * underlying tiny-bignum-c library.
   */
  struct bn inner;
} bn_t;

/**
 * @brief Indicates equality or inequality
 * in this bignumber implementation.
 * 
 */
typedef enum
{
  Bigger, /**< Indicates that a number is bigger than another */
  Smaller, /**< Indicates that a number is smaller than another */
  Equal, /**< Indicates that a number is equal to another */
} equality_t;

/**
 * @brief This module is a wrapper for 
 * <a href="https://github.com/kokke/tiny-bignum-c">this</a> 
 * library.
 * Functions in this module can be called by
 * invoking bn.`functionname`.
 */
struct bignum_h
{
  /**
   * @brief The `bn.new` function constructs
   * a new `bn_t` bignum struct and sets it 
   * to zero.
   * 
   * @return A new \link bn_t bn_t\endlink.
   */
  bn_t (*new)(void);
  /**
   * @brief The `bn.from_int` function 
   * constructs a new \link bn_t bn_t\endlink bignum struct 
   * from a 64-bit integer.
   * 
   * @param integer The 64-bit unsigned 
   * integer to be converted.
   */
  bn_t (*from_int)(uint64_t integer);
  /**
   * @brief Converts a \link bn_t bn_t\endlink to an integer.
   * 
   * @param bignumber A pointer to a \link bn_t bn_t\endlink .
   * 
   * @remark There might be a signed integer 
   * overflow if the `bignumber` is larger 
   * than the maximum value of the system's 
   * `int` type, which is typically an 
   * `int32_t`.
   */
  int (*to_int)(bn_t* bignumber);
  /**
   * @brief The `bn.from_nstr` function 
   * constructs a new \link bn_t bn_t\endlink
   * bignum struct from a `string` of `nbytes` length.
   * 
   * @param string The string to convert.
   * @param nbytes The length of the string 
   * in bytes.
   * 
   * @return A new \link bn_t bn_t\endlink.
   * 
   * @remarks 
   * - The `string` should be a hexadecimal 
   * representation of a number.
   * - It needs to consist of an even number 
   * of characters. Leading zeroes are allowed.
   * - The length of the `string` needs to be 
   * larger or equal to 8 characters. 
   * For smaller numbers the use of 
   * \link bignum_h.from_int bn.from_int\endlink 
   * is recommended.
   */
  bn_t (*from_nstr)(char* string, int nbytes);
  /**
   * @brief The `bn.from_cstr` function 
   * constructs a new `bn_t` bignum struct 
   * from a `cstring` that has been 
   * constructed by the \link cstring_h cstr\endlink module.
   * 
   * @param cstring A cstring from the \link cstring_h cstr\endlink module.
   * 
   * @remarks 
   * - The `string` should be a hexadecimal 
   * representation of a number.
   * - It needs to consist of an even number 
   * of characters. Leading zeroes are allowed.
   * - The length of the `string` needs to be 
   * larger or equal to 8 characters. 
   * For smaller numbers the use of 
   * \link bignum_h.from_int bn.from_int\endlink 
   * is recommended.
   */
  bn_t (*from_cstr)(char* cstring);
  /**
   * @brief Converts a `bignumber` 
   * to its hexadecimal string representation.
   * The result will be written to `string` 
   * and will contain at most `stringlen` 
   * bytes.
   * 
   * @param bignumber A pointer to a \link bn_t bn_t\endlink .
   * @param string Previously allocated memory.
   * @param stringlen The size of the allocated memory in bytes.
   * 
   * @remark The string length may be at most 
   * 256 bytes, this constraint is given by 
   * the tiny-bignum-c library, that is used 
   * as the underlying bignum library.
   */
  void (*to_nstr)(bn_t* bignumber, char* string, int stringlen);
  /**
   * @brief Converts a \link bn_t bn_t\endlink to its 
   * hexadecimal representation and returns 
   * it as a cstring from the 
   * \link cstring_h cstr\endlink module.
   */
  char* (*to_cstr)(bn_t* bignumber);

  /* Arithmetics */

  /**
   * @brief Applies the \f$a + b\f$ algebraic 
   * operation and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some\link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*add)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the \f$a - b\f$ algebraic 
   * operation and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*sub)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the \f$a \cdot b\f$ algebraic 
   * operation and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*mul)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the \f$\frac{a}{b}\f$ algebraic 
   * operation and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*div)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the \f$a \mod b\f$ algebraic 
   * operation and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*mod)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the following operations.
   * \f$ c = \frac{a}{b}\f$,
   * \f$ d = a\mod b\f$
   * where \f$ c\f$ is the result of the division and
   * \f$ d\f$ is the result of the modulo operation.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * @param c A pointer to a \link bn_t bn_t\endlink structure.
   * @param d A pointer to a \link bn_t bn_t\endlink structure.
   */
  void (*divmod)(bn_t* a, bn_t* b, bn_t* c, bn_t* d);

  /* Bitwise operations */
  /**
   * @brief Applies the binary and (&) operation (`a&b`)
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*and)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the binary or (|) operation (`a|b`).
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (* or)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the binary xor (^) operation (\f$a\oplus b\f$).
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*xor)(bn_t* a, bn_t* b);
  /**
   * @brief Applies the binary left bitshift operation (<<)
   * (`a << nbits`) and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param nbits Indicates how many bits to shift left.
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*lshift)(bn_t* a, int nbits);
  /**
   * @brief Applies the binary right bitshift operation (>>)
   * (`a >> nbits`) and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param nbits Indicates how many bits to shift right.
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*rshift)(bn_t* a, int nbits);

  /* Special operators and comparison */

  /**
   * @brief Compares `a` and `b` 
   * and returns the result. 
   * 
   * @return An enum defined as the type 
   * \link equality_t equality_t\endlink
   * that contains the members 
   * `Bigger`, `Smaller` and `Equal`.
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   */
  equality_t (*cmp)(bn_t* a, bn_t* b);
  /**
   * @brief Checks if `n` is zero.
   * 
   * @param n A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return `true` if `n` is the identity of zero and `false`
   * if this is not the case.
   */
  bool_t (*is_zero)(bn_t* n);
  /**
   * @brief Increments `n` by 1.
   * 
   * @param n A pointer to some \link bn_t bn_t\endlink .
   */
  void (*inc)(bn_t* n);
  /**
   * @brief Decremens `n` by 1.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * 
   */
  void (*dec)(bn_t* n);
  /**
   * @brief Applies the \f$ a^b\f$ algebraic 
   * operation and returns the result.
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * @param b A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*pow)(bn_t* a, bn_t* b);
  /**
   * @brief Computes the integer square root 
   * of `n` and returns the result.
   * For the result \f$ b\f$ the following relation
   * holds true. \f$ b\in \mathbb{N} \sqcup \{0\}\f$
   * 
   * @param a A pointer to some \link bn_t bn_t\endlink .
   * 
   * @return A new \link bn_t bn_t\endlink
   */
  bn_t (*isqrt)(bn_t* a);
  /**
   * @brief Assigns `src` to `dst`.
   * 
   * @param dst A pointer to some \link bn_t bn_t\endlink .
   * @param src A pointer to some \link bn_t bn_t\endlink .
   */
  void (*assign)(bn_t* dst, bn_t* src);
};

extern const struct bignum_h bn;

#endif /* BIGNUM_H */
