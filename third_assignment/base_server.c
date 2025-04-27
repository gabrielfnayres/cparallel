#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>



#define PORT 9090
#define BUFFER_SIZE 1024

int main(){
  int server_fd, client_fd;
  struct sockaddr_in address;

  int addrlen = sizeof(address);

  char buffer[BUFFER_SIZE];
  int valread;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("Falha na criação do socket");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  
  if(bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0){
    perror("falha no bind\n");
    exit(EXIT_FAILURE);
  }

  if(listen(server_fd, 1) < 0){
    perror("falha no listen\n");
    exit(EXIT_FAILURE);
  }
  printf("Servidor echo TCP iniciado na porta %d\n", PORT);
  printf("Aguardando conexão de cliente...\n");


  if((client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen)) < 0){
    perror("falha no accept\n");
    exit(EXIT_FAILURE);
  }

  printf("cliente conectado!\n");



  while(1){

    memset(buffer, 0, BUFFER_SIZE);

    valread = read(client_fd, buffer, BUFFER_SIZE);
    if(valread <= 0){
      printf("Cliente deconectou\n");
      break;
    }

    buffer[strcspn(buffer, "\n")] = 0;
    printf("Cliente: %s\n", buffer);
    
    if(strcmp(buffer, "EXIT") == 0){
      printf("recebido comando para encerrar. fechando servidor..\n");
      break;
    }

    send(client_fd, buffer, strlen(buffer), 0);
  }

  close(client_fd);
  close(server_fd);
  printf("Servidor encerrado\n");
  return 0;
}
