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
typedef enum
{
  false = 0,
  true = !false
} bool_t;
#define boolean_defined
#endif /* __cplusplus */
#endif /* boolean_defined */

#endif /* BOOLEAN_H */
