/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: colors.h
 * PURPOSE: Define colors for terminal output
 *************************************************************/

#ifndef COLORS_H
#define COLORS_H

struct styles
{
  const char *reset;
  const char *bold;
  const char *underline;
  const char *inverse;
};

struct normal_fg_colors
{
  const char *black;
  const char *red;
  const char *green;
  const char *yellow;
  const char *blue;
  const char *magenta;
  const char *cyan;
  const char *white;
};

struct normal_bg_colors
{
  const char *black;
  const char *red;
  const char *green;
  const char *yellow;
  const char *blue;
  const char *magenta;
  const char *cyan;
  const char *white;
};

struct strong_fg_colors
{
  const char *black;
  const char *red;
  const char *green;
  const char *yellow;
  const char *blue;
  const char *magenta;
  const char *cyan;
  const char *white;
};

struct strong_bg_colors
{
  const char *black;
  const char *red;
  const char *green;
  const char *yellow;
  const char *blue;
  const char *magenta;
  const char *cyan;
  const char *white;
};

struct colors_h
{
  const struct styles styles;
  const struct normal_fg_colors normal_fg;
  const struct normal_bg_colors normal_bg;
  const struct strong_fg_colors strong_fg;
  const struct strong_bg_colors strong_bg;
};

extern const struct colors_h colors;
#endif /* COLORS_H */
