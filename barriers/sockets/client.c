
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080


int main(){

  int sock = 0;
  char buffer[1024] = {0};
  struct sockaddr_in serv_addr;

  socklen_t len;

  if((sock= socket(AF_INET,SOCK_STREAM , 0)) < 0){
    perror("sokcet falhou\n");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);


  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
    perror("invalid address\n");
    exit(EXIT_FAILURE);
  }

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
    perror("failed to connect");
    close(sock);
    exit(EXIT_FAILURE);
  }

  read(sock, buffer, sizeof(buffer));

  printf("server message: %s\n", buffer);

  close(sock);
  return 0;
}
