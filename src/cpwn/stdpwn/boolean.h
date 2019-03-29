/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: boolean.h
 * PURPOSE: Define boolean type
 * GLOBAL VARIABLES:
 *
 * Variable        Type        Description
 * --------        ----        -----------
 * true            bool_t      Evaluates to !false
 * false           bool_t      Evaluates to 0
 *************************************************************/

#ifndef BOOLEAN_H
#define BOOLEAN_H

#ifndef boolean_defined
#ifndef __cplusplus
/**
 * @brief Defines a boolean type.
 */
typedef enum
{
  false = 0, /**< Indicates that something is not true. */
  true = !false /**< Indicates that something is not false. */
} bool_t;
#define boolean_defined
#endif /* __cplusplus */
#endif /* boolean_defined */

#endif /* BOOLEAN_H */
