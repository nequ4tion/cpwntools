/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: tcp.c
 * PURPOSE: Implement tcp sockets.
 *************************************************************/

#include "../../stdpwn/stdpwn.h"
#include "../networking.h"
#include "tcp.h"
#include <stdio.h>

#ifdef unix
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#elif defined(_WIN32)
#endif /* unix */

static sock_t
tcp_remote (const char *hostname, const port_t port)
{
  /* the socket that will be returned */
  sock_t ret = { 0 };

#ifdef unix
  /* used to store the result of an API request */
  int result;
  struct hostent *server;
  struct sockaddr_in serv_addr;
  /* tracks if the socket has been created */
  bool_t sock_created = false;

  /* request a new socket from the kernel */
  ret.sockfd = socket (AF_INET, SOCK_STREAM, 0);

  /* check if the socked file descriptor is valid, or if it is a defined error */
  if (ret.sockfd < 0) {
    goto FAILURE;
  }

  /* when the opening of a socket succeeded, the socket will be closed on future failure */
  sock_created = true;

  /* zero the sockaddr_in struct */
  memset (&serv_addr, 0, sizeof serv_addr);

  server = gethostbyname (hostname);

  if (server == NULL) {
    goto FAILURE;
  }

  /* copy h_addr to s_addr, yes it is bcopy(src, dest, n), as opposed to memcpy(dest, src, n) */
  bcopy ((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (port);
  /* connect to the remote host */
  result = connect (ret.sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr));

  if (result < 0)
    goto FAILURE;

#elif defined(_WIN32)
/* TODO: Implement windows */
#endif /* unix */

  /* fall through to succeed */
  ret.is_valid = true;
  return ret;

/* avoiding spaghetti code for a change with gotos */
FAILURE:
#ifdef unix
  if (sock_created)
    close (ret.sockfd);
  ret.sockfd = -1;
#elif defined(_WIN32)
  /* TODO: Implement windows */
#endif /* _WIN32 */

  ret.is_valid = false;
  return ret;

}

static ssz_t
tcp_send (sock_t *sock, const char *buf, const sz_t buflen)
{
  if (!sock->is_valid)
    return -1;
  ssz_t ret;
#ifdef unix
  // ret = (ssz_t)write (sock->sockfd, buf, buflen);
  ret = send (sock->sockfd, buf, buflen, 0);
#elif defined(_WIN32)
  /* TODO: Implement windows */
#endif /* unix */
  return ret;
}

static ssz_t
tcp_recv (sock_t *sock, char *buf, const sz_t len)
{
  if (!sock->is_valid)
    return -1;
  ssz_t ret;
#ifdef unix
  ret = (ssz_t)read (sock->sockfd, buf, len);
#elif defined(_WIN32)
  /* TODO: Implement windows */
#endif /* unix */
  return ret;
}

static void
tcp_close (sock_t *sock)
{
#ifdef unix
  close (sock->sockfd);
#elif defined(_WIN32)
  /* TODO: Implement windows */
#endif /* unix */
  sock->is_valid = false;
}

static int
tcp_shutdown(sock_t *sock)
{
  if (!sock->is_valid) {
    return -1;
  }
  #ifdef unix
    return shutdown(sock->sockfd, SHUT_RDWR);
  #elif defined(_WIN32)
    /* TODO: Implement windows */
  #endif /* unix */
}

static serv_sock_t
tcp_server (const char *hostname, const port_t port)
{
  sock_t ret = { 0 };

#ifdef unix
  /* used to store the result of an API request */
  int result;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  /* tracks if the socket has been openend */
  bool_t sock_open;
  int optval = 1;
  
  ret.sockfd = socket (AF_INET, SOCK_STREAM, 0);

  if (ret.sockfd < 0)
    goto FAILURE;
  /* now the file descriptor is marked as opened and will be closed appropriately on failure */
  sock_open = true;
  result = setsockopt (ret.sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof (int));
  if (result < 0)
    goto FAILURE;
  
  /* zero out the sockaddr_in struct */
  memset (&serv_addr, 0, sizeof serv_addr);

  serv_addr.sin_family = AF_INET;
  server = gethostbyname (hostname);

  if (server == NULL)
    goto FAILURE;

  bcopy ((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

  serv_addr.sin_port = htons (port);
  result = bind (ret.sockfd, (struct sockaddr *)&serv_addr, sizeof serv_addr);
  if (result < 0)
    goto FAILURE;
#elif defined(_WIN32)

#endif /* unix */
  ret.is_valid = true;
  return ret;

FAILURE:
#ifdef unix
  if (sock_open)
    close (ret.sockfd);
#endif /* unix */
  ret.is_valid = false;
  return ret;
}

static sock_t
next_client (serv_sock_t *server, const int listenlen)
{
  sock_t ret = { 0 };
#ifdef unix
  struct sockaddr_in client_addr;
  socklen_t clientlen;

  listen (server->sockfd, listenlen);
  clientlen = sizeof (client_addr);
  ret.sockfd
      = accept (server->sockfd, (struct sockaddr *)&client_addr, &clientlen);
  if (ret.sockfd < 0)
    goto FAILURE;
#elif defined(_WIN32)

#endif /* unix */

  ret.is_valid = true;
  return ret;

FAILURE:
  ret.is_valid = false;
  return ret;
}

static bool_t
valid (const sock_t sock)
{
  return sock.is_valid;
}

const struct tcp_h tcp = { .remote = tcp_remote,
                           .send = tcp_send,
                           .close = tcp_close,
                           .recv = tcp_recv,
                           .shutdown = tcp_shutdown,
                           .server = tcp_server,
                           .next_client = next_client,
                           .valid = valid };
