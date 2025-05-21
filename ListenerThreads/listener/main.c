#include "network_utils.h"
#include <pthread.h>
#include <stdio.h>

void recieve_function(char *buffer, unsigned int buffer_size, char *ip,
                      unsigned int port) {
  printf("%s() : pkt recvd = %s, pkt size = %u\n", __FUNCTION__, buffer,
         buffer_size);
}

pthread_t *handle1, *handle2;

int main() {

  handle1 = create_listener_thread("127.0.0.1", 3000, recieve_function);

  handle2 = create_listener_thread("127.0.0.1", 5000, recieve_function);

  pthread_exit(0);

  return 0;
}
