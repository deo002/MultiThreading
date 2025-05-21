#ifndef __NETWORK_UTILS__
#define __NETWORK_UTILS__

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_RECV_BUFFER_SIZE 256

#define IP_BUFFER_SIZE 16

typedef void (*recv_fn_cb)(char *, unsigned int);

typedef struct network_thread {
  char ip_addr[IP_BUFFER_SIZE];
  unsigned int port;
  recv_fn_cb recv_fn;
} network_thread_t;

pthread_t *create_listener_thread(char *ip_addr, unsigned int port,
                                  recv_fn_cb recv_fn);

static void *udp_server_create_and_start(void *args);

char *network_covert_ip_n_to_p(uint32_t ip_addr, char *output_buffer);

uint32_t network_covert_ip_p_to_n(char *ip_addr);

#endif
