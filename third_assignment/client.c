#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"  // Padrão para localhost

/**
 * Programa cliente para o servidor Echo TCP
 * 
 * @param argc Número de argumentos da linha de comando
 * @param argv Array de strings com argumentos da linha de comando
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char input[BUFFER_SIZE];
    const char *server_ip = SERVER_IP;
    
    // Usa o argumento da linha de comando para o IP do servidor, se fornecido
    if (argc > 1) {
        server_ip = argv[1];
    }
    
    // Cria o socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Erro ao criar socket\n");
        return -1;
    }
    
    // Configura o endereço do servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Converte o endereço IP de texto para binário
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("Endereço inválido ou não suportado\n");
        return -1;
    }
    
    // Conecta ao servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Conexão falhou\n");
        return -1;
    }
    
    printf("Conectado ao servidor %s:%d\n", server_ip, PORT);
    printf("Digite mensagens para enviar (EXIT para sair):\n");
    
    // Loop principal do cliente
    while (1) {
        printf("> ");
        fgets(input, BUFFER_SIZE, stdin);
        
        // Remove o caractere de nova linha
        input[strcspn(input, "\n")] = 0;
        
        // Envia mensagem para o servidor
        send(sock, input, strlen(input), 0);
        
        // Verifica o comando de saída
        if (strcmp(input, "EXIT") == 0) {
            printf("Encerrando cliente...\n");
            break;
        }
        
        // Limpa o buffer
        memset(buffer, 0, sizeof(buffer));
        
        // Recebe eco do servidor
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Servidor desconectou\n");
            break;
        }
        
        printf("Eco: %s\n", buffer);
    }
    
    // Fecha o socket
    close(sock);
    printf("Cliente encerrado\n");
    
    return 0;
}
