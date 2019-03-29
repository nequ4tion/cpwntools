/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: tcp.h
 * PURPOSE: Implements tcp sockets.
 *************************************************************/

#ifndef TCP_H
#define TCP_H

#include "../../stdpwn/stdpwn.h"
#include "../networking.h"

typedef struct SOCKET_TYPE serv_sock_t;

/**
 * @brief A module implementing tcp sockets.
 * Functions in this module can be called by
 * invoking tcp.`functionname`
 */
struct tcp_h
{
  /**
   * @brief Connects to a host on the specified port.
   *
   * @param hostname The host to connect to
   * @param port The port to connect to
   * @return sock_t Can be closed with tcp.close()
   */
  sock_t (*remote)(const char*, const port_t);
  /**
   * @brief Sends the buffer to a host
   *
   * @param sock Pointer to a valid socket
   * @param buf The buffer to send
   * @param buflen The length of the buffer
   * @return Bytes sent
   */
  ssz_t (*send)(sock_t*,
                const char*,
                const
#ifdef unix
                sz_t
#elif defined(_WIN32)
                int
#endif /* unix */
  );
  /**
   * @brief Closes a socket.
   *
   * @param sock Pointer to a valid socket
   */
  void (*close)(sock_t*);
  /**
   * @brief Receives a number of bytes
   *
   * @param sock Pointer to a valid socket
   * @param buf The buffer to write to
   * @param buflen The length of the buffer
   * @return Bytes received
   */
  ssz_t (*recv)(sock_t*,
                char*,
                const
#ifdef unix
                sz_t
#elif defined(_WIN32)
                int
#endif /* unix */
  );
  /**
   * @brief Shuts down both ends of the connection
   *
   * @param sock The socket on which the connection should be shut down.
   */
  int (*shutdown)(sock_t*);
  /**
   * @brief Creates a server socket.
   *
   * @param hostname Hostname to bind to
   * @param port Port to bind to
   * @return Server socket
   */
  serv_sock_t (*server)(const char*, const port_t);
  /**
   * @brief Returns the next client
   *
   * @param sock Pointer to a valid server socket
   * @param listenlen Maximum length of the connection queue
   * @return Socket connected to a client
   */
  sock_t (*next_client)(serv_sock_t*, const int);
  /**
   * @brief Checks if a socket is valid
   *
   * @param sock Pointer to a socket
   * @return true or false depending on validity of the socket
   */
  bool_t (*valid)(const sock_t);
};

extern const struct tcp_h tcp;

#endif /* TCP_H */
