/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: colors.c
 * PURPOSE: Define colors for terminal output
 *************************************************************/

#include "colors.h"

#define ESC_SEQU "\033"

const struct colors_h colors = { .styles = { .reset = ESC_SEQU "[0m",
                                             .bold = ESC_SEQU "[1m",
                                             .underline = ESC_SEQU "[4m",
                                             .inverse = ESC_SEQU "[7m" },
                                 .normal_fg = { .black = ESC_SEQU "[30m",
                                                .red = ESC_SEQU "[31m",
                                                .green = ESC_SEQU "[32m",
                                                .yellow = ESC_SEQU "[33m",
                                                .blue = ESC_SEQU "[34m",
                                                .magenta = ESC_SEQU "[35m",
                                                .cyan = ESC_SEQU "[36m",
                                                .white = ESC_SEQU "[37m" },
                                 .normal_bg = { .black = ESC_SEQU "[40m",
                                                .red = ESC_SEQU "[41m",
                                                .green = ESC_SEQU "[42m",
                                                .yellow = ESC_SEQU "[43m",
                                                .blue = ESC_SEQU "[44m",
                                                .magenta = ESC_SEQU "[45m",
                                                .cyan = ESC_SEQU "[46m",
                                                .white = ESC_SEQU "[47m" },
                                 .strong_fg = { .black = ESC_SEQU "[90m",
                                                .red = ESC_SEQU "[91m",
                                                .green = ESC_SEQU "[92m",
                                                .yellow = ESC_SEQU "[93m",
                                                .blue = ESC_SEQU "[94m",
                                                .magenta = ESC_SEQU "[95m",
                                                .cyan = ESC_SEQU "[96m",
                                                .white = ESC_SEQU "[97m" },
                                 .strong_bg = { .black = ESC_SEQU "[100m",
                                                .red = ESC_SEQU "[101m",
                                                .green = ESC_SEQU "[102m",
                                                .yellow = ESC_SEQU "[103m",
                                                .blue = ESC_SEQU "[104m",
                                                .magenta = ESC_SEQU "[105m",
                                                .cyan = ESC_SEQU "[106m",
                                                .white = ESC_SEQU "[107m" } };
