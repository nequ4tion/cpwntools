/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: archdefs.h
 * PURPOSE: Define architecture specific information
 * GLOBAL VARIABLES:
 *
 * Variable                Type        Description
 * --------                ----        -----------
 * X86_64_ARCH             int         Set to 1 if on x86_64
 * X86_ARCH                int         Set to 1 if on x86
 * IA64_ARCH               int         Set to 1 if on IA64
 * PPC_ARCH                int         Set to 1 if on PPC
 * PPC64_ARCH              int         Set to 1 if on PPC64
 * SPARC_ARCH              int         Set to 1 if on SPARC
 * UNDEFINED_ENDIAN        int         Set to 1 if no clear endianess
 * PROCESSOR_LITTLE_ENDIAN int         Set to 1 if little endian
 * PROCESSOR_BIG_ENDIAN    int         Set to 1 if big endian
 *************************************************************/

#ifndef ARCHDEFS_H
#define ARCHDEFS_H

#include "stdpwn.h"

enum ARCH
{
  X86,
  X86_64,
  ARM,
  ARM64,
  AVR
};

enum ENDIAN
{
  BIG,
  SMALL
};

#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64)              \
    || defined(__amd64__) || defined(_M_AMD64) || defined(_M_X64)
#define X86_64_ARCH 1
#endif

#if defined(__i386__) || defined(i386) || defined(__i386) || defined(_M_IX86) \
    || defined(_X86_)
#define X86_ARCH 1
#endif

#if defined(ia64) || defined(__ia64) || defined(__ia64__)                     \
    || defined(__IA64__) || defined(__itanium__) || defined(_M_IA64)
#define IA64_ARCH 1
#endif

#if defined(_ARCH_PPC) || defined(_POWER) || defined(powerpc)                 \
    || defined(__powerpc) || defined(__powerpc__) || defined(__PowerPC__)     \
    || defined(__POWERPC__) || defined(PPC) || defined(__ppc__)               \
    || defined(__PPC) || defined(__PPC__)
#define PPC_ARCH 1
#endif

#if defined(_ARCH_PPC64) || defined(__powerpc64__) || defined(__ppc64)        \
    || defined(__ppc64__) || defined(__PPC64__)
#ifdef PPC_ARCH
#undef PPC_ARCH
#endif
#define PPC64_ARCH 1
#endif

#if defined(sparc) || defined(__sparc) || defined(__sparc__)                  \
    || defined(__sparc64__)
#define SPARC_ARCH 1
#endif

#if defined(IA64_ARCH)
/* Must be evaluated at runtime */
#define UNDEFINED_ENDIAN 1
#endif

#if defined(X86_ARCH) || defined(X86_64_ARCH)
#define PROCESSOR_LITTLE_ENDIAN 1
#endif

#if defined(PPC_ARCH) || defined(PPC64_ARCH) || defined(SPARC_ARCH)
#define PROCESSOR_BIG_ENDIAN 1
#endif

#endif /* ARCHDEFS_H */
