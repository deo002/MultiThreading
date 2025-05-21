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
static void handle_free_args(void *args) { free(args); }

static void handle_close_socket(void *arg) {
  int socketfd = *(int *)arg;
  close(socketfd);
}

static void handle_free_buffer(void *buffer) { free(buffer); }

static void *udp_server_create_and_start(void *args) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

  pthread_cleanup_push(handle_free_args, args);

  network_thread_t *_args = args;

  int socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socketfd < 0) {
    perror("Unable to open socket");
    pthread_exit(0);
  }

  pthread_cleanup_push(handle_close_socket, &socketfd);

  struct sockaddr_in listener_addr, client_addr;
  listener_addr.sin_family = AF_INET;
  listener_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  listener_addr.sin_port = htons(_args->port);

  if (bind(socketfd, (struct sockaddr *)&listener_addr, sizeof(listener_addr)) <
      0) {
    perror("Unable to bind socket");
    pthread_exit(0);
  }

  char *recv_buffer = calloc(1, MAX_RECV_BUFFER_SIZE);

  pthread_cleanup_push(handle_free_buffer, recv_buffer);

  int bytes_recvd = 0, len = sizeof(client_addr);

  while (1) {
    memset(recv_buffer, 0, MAX_RECV_BUFFER_SIZE);

    bytes_recvd = recvfrom(socketfd, recv_buffer, MAX_RECV_BUFFER_SIZE, 0,
                           (struct sockaddr *)&client_addr, (socklen_t *)&len);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, (socklen_t)len);

    _args->recv_fn(recv_buffer, bytes_recvd);
    pthread_testcancel();
  }

  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);

  return NULL;
}
