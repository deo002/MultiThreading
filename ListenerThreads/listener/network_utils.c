#include "network_utils.h"
#include <sys/socket.h>

pthread_t *create_listener_thread(char *ip_addr, unsigned int port,
                                  recv_fn_cb recv_fn) {
  pthread_t *handle = calloc(1, sizeof(pthread_t));

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  network_thread_t *args = calloc(1, sizeof(network_thread_t));
  strcpy(args->ip_addr, ip_addr);
  args->port = port;
  args->recv_fn = recv_fn;

  pthread_create(handle, &attr, udp_server_create_and_start, (void *)args);

  return handle;
}

static void *udp_server_create_and_start(void *args) {
  network_thread_t *_args = args;

  int socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socketfd < 0) {
    perror("Unable to open socket");
  }

  struct sockaddr_in listener_addr, client_addr;
  listener_addr.sin_family = AF_INET;
  listener_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  listener_addr.sin_port = htons(_args->port);

  if (bind(socketfd, (struct sockaddr *)&listener_addr, sizeof(listener_addr)) <
      0) {
    perror("Unable to bind socket");
  }

  char *recv_buffer = calloc(1, MAX_RECV_BUFFER_SIZE);

  int bytes_recvd = 0, len = sizeof(client_addr);

  while (1) {
    memset(recv_buffer, 0, MAX_RECV_BUFFER_SIZE);

    bytes_recvd = recvfrom(socketfd, recv_buffer, MAX_RECV_BUFFER_SIZE, 0,
                           (struct sockaddr *)&client_addr, (socklen_t *)&len);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, (socklen_t)len);

    _args->recv_fn(recv_buffer, bytes_recvd);
  }

  return NULL;
}
