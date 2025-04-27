/// server 


#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char* message = "hello client!\n";

  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("socket falhou\n");
    exit(EXIT_FAILURE);
  }


  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
    perror("bind flahou");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if(listen(server_fd, 3) < 0){
    perror("listen falhou\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
    perror("accept failed\n");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  send(new_socket, message,strlen(message), 0);
  printf("sent message\n");
  close(new_socket);
  close(server_fd);
  return 0;
}
