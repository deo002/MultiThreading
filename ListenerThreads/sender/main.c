#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

  if (argc < 4) {
    fprintf(stderr,
            "IP, port and message must be passed as command line arguments");
  }

  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in servaddr;

  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  servaddr.sin_port = htons(strtoul(argv[2], NULL, 10));
  servaddr.sin_family = AF_INET;

  if (sendto(socketfd, argv[3], strlen(argv[3]), 0,
             (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Unable to send message: ");
  }

  close(socketfd);

  return 0;
}
