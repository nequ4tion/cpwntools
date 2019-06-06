/* Copyright (c) 2018 Nequ4tion */
/* See LICENSE for licensing information */

/* ***********************************************************
 * NAME: tcp.c
 * PURPOSE: Implement tcp sockets.
 *************************************************************/

#include "tcp.h"
#include "../../stdpwn/stdpwn.h"
#include "../networking.h"
#include <stdio.h>

#ifdef unix
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include "../../stdpwn/stdtypes.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <inttypes.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif /* unix */

#ifdef _WIN32
struct win_sock_status {
  bool_t initialized;
  sz_t number_of_opened_sockets;
  WSADATA wsadata;
};
static struct win_sock_status winsock_status = { .initialized = false,
                                                 .number_of_opened_sockets = 0,
                                                 .wsadata = { 0 } };
static HANDLE winsock_status_lock = NULL; 
#endif /* _WIN32 */

static sock_t
tcp_remote(const char* hostname, const port_t port)
{
  /* the socket that will be returned */
  sock_t ret = { 
#ifdef unix
    .sockfd = 0,
#elif defined(_WIN32)
    .sock = INVALID_SOCKET,
#endif /* unix */ 
    .is_valid = false };

#ifdef unix
  /* used to store the result of an API request */
  int result;
  struct hostent* server;
  struct sockaddr_in serv_addr;
  /* tracks if the socket has been created */
  bool_t sock_created = false;

  /* request a new socket from the kernel */
  ret.sockfd = socket(AF_INET, SOCK_STREAM, 0);

  /* check if the socked file descriptor is valid, or if it is a defined error
   */
  if (ret.sockfd < 0) {
    goto FAILURE;
  }

  /* when the opening of a socket succeeded, the socket will be closed on future
   * failure */
  sock_created = true;

  /* zero the sockaddr_in struct */
  memset(&serv_addr, 0, sizeof serv_addr);

  server = gethostbyname(hostname);

  if (server == NULL) {
    goto FAILURE;
  }

  /* copy h_addr to s_addr, yes it is bcopy(src, dest, n), as opposed to
   * memcpy(dest, src, n) */
  bcopy(
    (char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  /* connect to the remote host */
  result = connect(ret.sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  if (result < 0)
    goto FAILURE;

#elif defined(_WIN32)
  struct addrinfo *result = NULL,
                  /* the ptr will be used later to loop through all possible connections */
                  *ptr = NULL,
                  /* hints will be initialized with 0 before use later on */
                  hints;
  int winapi_result = 0;
  DWORD winsock_status_mutex_lock_result;
  /* tracks if addrinfo *result has been initialized */
  bool_t result_initialized;
  /* the windows API for some reason demands port numbers encoded in ASCII represenation;
   * 65535 is the highest port number possible (2^16 - 1 is the highest 16-bit unsigned integer) with an ASCII
   * encoding length of 5 characters, therefore 6 bytes (including the NUL-terminator) are needed to store 
   * a port number for the windows API
   */
  char ascii_port[6] = { 0 };

  if (winsock_status_lock == NULL) {
    /* winsock_status_lock is not initialized yet, initializing */
    winsock_status_lock = CreateMutex(NULL, /* default security attributes */
                                      FALSE, /* not owned initially */
                                      NULL /* unnamed mutex */
                                    );

    if (winsock_status_lock == NULL) {
      /* an error occured */
      goto FAILURE;
    }
  }
  /* winsock_status_lock is initialized since the check for NULL did not evaluate to to true */

  /* requesting the lock for the mutex */
  winsock_status_mutex_lock_result = WaitForSingleObject( winsock_status_lock, /* handle to the lock */
                                                          INFINITE /* no time out */
                                                        );

  /* check if the ownership over the mutex has been aquired */
  if (winsock_status_mutex_lock_result == WAIT_OBJECT_0) {
    /* ownership over the mutex aquired */

    /* check if winsock is initialized, if not => initialize */
    if (!winsock_status.initialized) {
      /* startup wsa */
      winapi_result = WSAStartup(MAKEWORD(2,2), &winsock_status.wsadata);

      /* a non-zero result indicates an error */
      if (winapi_result != 0) {
        /* an error occured during wsa startup */
        goto FAILURE;
      }

      /* set to true since no errors occured during startup */
      winsock_status.initialized = true;
    }

    /* create a new socket and increment the number of opened sockets */

    /* initialize the memory of hints with 0 */
    ZeroMemory(&hints, sizeof(hints));

    /* default settings for the TCP/IP protocol */
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* convert the port number from the arguments to ASCII, 6 bytes are available;
     * see explanation in the declaration of ascii_port
     */
    snprintf(ascii_port,
             sizeof(ascii_port) /* 6 */,
             "%" PRIu16,
             port);

    /* get information about the user-given data */
    winapi_result = getaddrinfo(hostname, ascii_port, &hints, &result);

    /* results differing from 0 indicate an error */
    if (winapi_result != 0) {
      goto FAILURE;
    }

    result_initialized = true;

    /* try to connect until one succeeds */
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

      /* create a HANDLE to the socket */
      ret.sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

      if (ret.sock == INVALID_SOCKET) {
        /* an error occurred, attempting to clean up */

        if (winsock_status.number_of_opened_sockets == 0) {
          /* if there are currently no sockets open, clean WSA up */
          WSACleanup();
          winsock_status.initialized = false;
          memset(&winsock_status.wsadata, 0, sizeof(winsock_status.wsadata));
        }
        goto FAILURE;
      }

      /* attempting to connect to the server */
      winapi_result = connect(ret.sock, ptr->ai_addr,  (int) ptr->ai_addrlen);
      if (winapi_result == SOCKET_ERROR) {
        closesocket(ret.sock);
        ret.sock = INVALID_SOCKET;
        continue;
      }
      break;
    }
    /* increment the number of opened sockets by 1 */
    winsock_status.number_of_opened_sockets++;
  }
  /* this is the else for the check if the mutex lock has been successfully aquired */
  else if (winsock_status_mutex_lock_result == WAIT_ABANDONED) {
    /* a thread abandoned the lock and left it locked, giving up here */
    goto FAILURE;
  }

  /* the socket has been opened, releasing the lock for other threads */
  ReleaseMutex(winsock_status_lock);
  
  freeaddrinfo(result);
#endif /* unix */

  /* fall through to succeed */
  ret.is_valid = true;
  return ret;

/* avoiding spaghetti code for a change with gotos */
FAILURE:
#ifdef unix
  if (sock_created)
    close(ret.sockfd);
  ret.sockfd = -1;
#elif defined(_WIN32)
  /* on windows there is no socket to close since it is the last call to the windows API
   * and if it fails there has no socket been opened
   */

  if (result_initialized) {
    freeaddrinfo(result);
  }

  /* release the mutex lock */
  ReleaseMutex(winsock_status_lock);

  /* to be sure, set the returning value to INVALID_SOCKET */
  ret.sock = INVALID_SOCKET;
#endif /* _WIN32 */

  ret.is_valid = false;
  return ret;
}

static ssz_t
tcp_send(sock_t* sock, const char* buf, const 
#ifdef unix
  sz_t
#elif defined(_WIN32)
  int
#endif /* unix */
  buflen)
{
  if (!sock->is_valid)
    return -1;
  ssz_t ret;
#ifdef unix
  // ret = (ssz_t)write (sock->sockfd, buf, buflen);
  ret = (ssz_t) send(sock->sockfd, buf, buflen, 0);
#elif defined(_WIN32)
  ret = (ssz_t) send(sock->sock, buf, buflen, 0);
#endif /* unix */
  return ret;
}

static ssz_t
tcp_recv(sock_t* sock, char* buf, const
#ifdef unix
  sz_t
#elif defined(_WIN32)
  int
#endif /* unix */
  len)
{
  if (!sock->is_valid)
    return -1;
  ssz_t ret;
#ifdef unix
  ret = (ssz_t)read(sock->sockfd, buf, len);
#elif defined(_WIN32)
  ret = (ssz_t)recv(sock->sock, buf, len, 0);
#endif /* unix */
  return ret;
}

static void
tcp_close(sock_t* sock)
{
#ifdef unix
  close(sock->sockfd);
#elif defined(_WIN32)
  DWORD winsock_status_mutex_lock_result;

  /* requesting the lock for the mutex */
  winsock_status_mutex_lock_result = WaitForSingleObject( winsock_status_lock, /* handle to the lock */
                                                          INFINITE /* no time out */
                                                        );

  /* check if the ownership over the mutex has been aquired */
  if (winsock_status_mutex_lock_result == WAIT_OBJECT_0) {
    /* close the socket */
    closesocket(sock->sock);
    /* decrement the number of opened sockets by 1 */
    winsock_status.number_of_opened_sockets--;
    if (winsock_status.number_of_opened_sockets == 0) {
      /* no more sockets remaining, cleaning up */
      WSACleanup();
      winsock_status.initialized = false;
      memset(&winsock_status.wsadata, 0, sizeof(winsock_status.wsadata));
      ReleaseMutex(winsock_status_lock);
    }
  } else if (winsock_status_mutex_lock_result == WAIT_ABANDONED) {
    /* an error has occurred */
    return;
  }
#endif /* unix */
  sock->is_valid = false;
}

static int
tcp_shutdown(sock_t* sock)
{
  if (!sock->is_valid) {
    return -1;
  }
#ifdef unix
  return shutdown(sock->sockfd, SHUT_RDWR);
#elif defined(_WIN32)
  return shutdown(sock->sock, SD_BOTH);
#endif /* unix */
}

static serv_sock_t
tcp_server(const char* hostname, const port_t port)
{
  /* tracks if the socket has been openend */
  bool_t sock_open = false;

  sock_t ret = {
#ifdef unix
     .sockfd = 0, 
#elif defined(_WIN32)
     .sock = INVALID_SOCKET,
#endif /* unix */
     .is_valid = false };

#ifdef unix
  /* used to store the result of an API request */
  int result;
  struct sockaddr_in serv_addr;
  struct hostent* server;
  int optval = 1;

  ret.sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (ret.sockfd < 0)
    goto FAILURE;
  /* now the file descriptor is marked as opened and will be closed
   * appropriately on failure */
  sock_open = true;
  result = setsockopt(
    ret.sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(int));
  if (result < 0)
    goto FAILURE;

  /* zero out the sockaddr_in struct */
  memset(&serv_addr, 0, sizeof serv_addr);

  serv_addr.sin_family = AF_INET;
  server = gethostbyname(hostname);

  if (server == NULL)
    goto FAILURE;

  bcopy((char*)server->h_addr_list[0],
        (char*)&serv_addr.sin_addr.s_addr,
        server->h_length);

  serv_addr.sin_port = htons(port);
  result = bind(ret.sockfd, (struct sockaddr*)&serv_addr, sizeof serv_addr);
  if (result < 0)
    goto FAILURE;
#elif defined(_WIN32)
  /* the windows API for some reason demands port numbers encoded in ASCII represenation;
   * 65535 is the highest port number possible (2^16 - 1 is the highest 16-bit unsigned integer) with an ASCII
   * encoding length of 5 characters, therefore 6 bytes (including the NUL-terminator) are needed to store
   * a port number for the windows API
   */
  char ascii_port[6] = { 0 };
  DWORD winsock_status_mutex_lock_result;
  struct addrinfo *result = NULL;
  struct addrinfo hints;
  int winapi_result = 0;
  /* tracks if addrinfo *result has been initialized */
  bool_t result_initialized;

  /* convert the port number from the arguments to ASCII, 6 bytes are available;
   * see explanation in the declaration of ascii_port
   */
  snprintf(ascii_port,
           sizeof(ascii_port) /* 6 */,
           "%" PRIu16,
           port);

  /* requesting the lock for the mutex */
  winsock_status_mutex_lock_result = WaitForSingleObject( winsock_status_lock, /* handle to the lock */
                                                          INFINITE /* no time out */
                                                        );

  /* check if the ownership over the mutex has been aquired */
  if (winsock_status_mutex_lock_result == WAIT_OBJECT_0) {
    if (!winsock_status.initialized) {
      /* startup WSA */
      winapi_result = WSAStartup(MAKEWORD(2,2), &winsock_status.wsadata);

      /* non-zero indicates an error */
      if (winapi_result != 0) {
        goto FAILURE;
      }

      /* now that WSA is initialized, set it to true */
      winsock_status.initialized = true;
    }
    
    /* initialize the unused fields in hints with 0 */
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    /* getaddrinfo of the hostname and port with specified protocol information from hints */
    winapi_result = getaddrinfo(hostname, ascii_port, &hints, &result);

    if (winapi_result != 0) {
      goto FAILURE;
    }

    result_initialized = true;

    /* initialize the socket that will be used for the server */
    ret.sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if(ret.sock == INVALID_SOCKET) {
      goto FAILURE;
    }

    sock_open = true;

    /* bind the socket to the given address and port */
    winapi_result = bind(ret.sock, result->ai_addr, (int)result->ai_addrlen);

    if (winapi_result == SOCKET_ERROR) {
      goto FAILURE;
    }

    /* free the memory allocated by getaddrinfo */
    freeaddrinfo(result);

  } else if (winsock_status_mutex_lock_result == WAIT_ABANDONED) {
    /* an error has occurred */
    goto FAILURE;
  }

#endif /* unix */
  ret.is_valid = true;
  return ret;

FAILURE:
#ifdef unix
  if (sock_open)
    close(ret.sockfd);
#elif defined(_WIN32)
  /* the mutex is still unlocked, attempt cleanup */
  if (winsock_status.number_of_opened_sockets == 0) {
    /* no more sockets open, uninitialize winsock */
    WSACleanup();
    winsock_status.initialized = false;
    memset(&winsock_status.wsadata, 0, sizeof(winsock_status.wsadata));
  }

  if (result_initialized) {
    freeaddrinfo(result);
  }

  if (sock_open) {
    closesocket(ret.sock);
  }

  ReleaseMutex(winsock_status_lock);
#endif /* unix */
  ret.is_valid = false;
  return ret;
}

static sock_t
next_client(serv_sock_t* server, const int listenlen)
{
  /* the socket to be returned */
  sock_t ret = {
#ifdef unix     
     .sockfd = 0,
#elif defined(_WIN32)
     .sock = INVALID_SOCKET,
#endif /* unix */
     .is_valid = false };

#ifdef unix
  struct sockaddr_in client_addr;
  socklen_t clientlen;

  listen(server->sockfd, listenlen);
  clientlen = sizeof(client_addr);
  ret.sockfd =
    accept(server->sockfd, (struct sockaddr*)&client_addr, &clientlen);
  if (ret.sockfd < 0)
    goto FAILURE;
#elif defined(_WIN32)
  int winapi_result = 0;

  winapi_result = listen(server->sock, listenlen);

  if (winapi_result == SOCKET_ERROR) {
    goto FAILURE;
  }

  ret.sock = accept(server->sock, NULL, NULL);

  if (ret.sock == INVALID_SOCKET) {
    goto FAILURE;
  }
#endif /* unix */

  ret.is_valid = true;
  return ret;

FAILURE:
  ret.is_valid = false;
  return ret;
}

static bool_t
valid(const sock_t sock)
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
