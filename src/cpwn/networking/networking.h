/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: networking.h
 * PURPOSE: Define headers for networking
 *************************************************************/

#ifndef NETWORKING_H
#define NETWORKING_H

#ifdef unix
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#endif /* unix */

#include "../stdpwn/stdpwn.h"

typedef unsigned short int port_t;

typedef struct SOCKET_TYPE
{
#ifdef unix
  int sockfd;

#elif defined(_WIN32)
 SOCKET sock;
#endif /* unix */
  bool_t is_valid;
} sock_t;

#include "tcp/tcp.h"

#endif /* NETWORKING_H */
