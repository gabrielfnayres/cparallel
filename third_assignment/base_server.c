#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 9090
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define LOG_FILE "server_log.txt"

/**
 * Estrutura de monitor para registro de log thread-safe
 */
typedef struct {
    pthread_mutex_t mutex;
    FILE *log_file;
} LogMonitor;

/**
 * Informações de conexão do cliente
 */
typedef struct {
    int socket_fd;
    struct sockaddr_in address;
    int id;
} client_t;

/**
 * Variáveis globais
 */
LogMonitor logger;
int client_count = 0;
pthread_mutex_t client_count_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Inicializa o monitor de logger
 */
void init_logger() {
    pthread_mutex_init(&logger.mutex, NULL);
    logger.log_file = fopen(LOG_FILE, "a");
    if (logger.log_file == NULL) {
        perror("Erro ao abrir arquivo de log");
        exit(EXIT_FAILURE);
    }
}

/**
 * Fecha o logger
 */
void close_logger() {
    pthread_mutex_lock(&logger.mutex);
    if (logger.log_file) {
        fclose(logger.log_file);
        logger.log_file = NULL;
    }
    pthread_mutex_unlock(&logger.mutex);
    pthread_mutex_destroy(&logger.mutex);
}

/**
 * Função de registro thread-safe
 * 
 * @param client_ip Endereço IP do cliente
 * @param message Mensagem a ser registrada
 */
void log_message(const char *client_ip, const char *message) {
    time_t now;
    struct tm *time_info;
    char timestamp[26];
    
    time(&now);
    time_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
    
    pthread_mutex_lock(&logger.mutex);
    fprintf(logger.log_file, "[%s] %s: %s\n", timestamp, client_ip, message);
    fflush(logger.log_file);
    pthread_mutex_unlock(&logger.mutex);
}

/**
 * Função de thread para lidar com conexões de clientes
 * 
 * @param arg Ponteiro para a estrutura do cliente
 * @return NULL após a conclusão da thread
 */
void *handle_client(void *arg) {
    client_t *client = (client_t *)arg;
    int client_fd = client->socket_fd;
    char buffer[BUFFER_SIZE] = {0};
    int valread;
    
    // Obtém o endereço IP do cliente
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client->address.sin_addr), client_ip, INET_ADDRSTRLEN);
    
    printf("Cliente %d conectado de %s\n", client->id, client_ip);
    
    // Registra a conexão do cliente
    char connect_msg[100];
    sprintf(connect_msg, "Cliente %d conectado", client->id);
    log_message(client_ip, connect_msg);
    
    // Loop de eco
    while (1) {
        // Limpa o buffer
        memset(buffer, 0, BUFFER_SIZE);
        
        // Recebe mensagem do cliente
        valread = read(client_fd, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Cliente %d desconectado\n", client->id);
            break;
        }
        
        // Remove o caractere de nova linha
        buffer[strcspn(buffer, "\n")] = 0;
        printf("Cliente %d: %s\n", client->id, buffer);
        
        // Registra a mensagem recebida
        log_message(client_ip, buffer);
        
        // Verifica o comando de saída
        if (strcmp(buffer, "EXIT") == 0) {
            printf("Cliente %d solicitou sair\n", client->id);
            break;
        }
        
        // Envia resposta de eco de volta para o cliente
        send(client_fd, buffer, strlen(buffer), 0);
    }
    
    // Registra a desconexão do cliente
    char disconnect_msg[100];
    sprintf(disconnect_msg, "Cliente %d desconectado", client->id);
    log_message(client_ip, disconnect_msg);
    
    // Fecha o socket do cliente
    close(client_fd);
    
    // Decrementa o contador de clientes
    pthread_mutex_lock(&client_count_mutex);
    client_count--;
    pthread_mutex_unlock(&client_count_mutex);
    
    free(client);
    pthread_exit(NULL);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t threads[MAX_CLIENTS];
    
    // Inicializa o logger
    init_logger();
    
    // Cria o socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha na criação do socket");
        exit(EXIT_FAILURE);
    }
    
    // Define opções do socket para reutilização de endereço
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Falha ao configurar socket");
        exit(EXIT_FAILURE);
    }
    
    // Configura o endereço do socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Vincula o socket ao endereço e porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha no bind");
        exit(EXIT_FAILURE);
    }
    
    // Configura o socket para escutar
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Falha no listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Servidor echo TCP iniciado na porta %d\n", PORT);
    printf("Aguardando conexões de clientes...\n");
    
    // Loop principal do servidor
    while (1) {
        // Verifica se o número máximo de clientes foi atingido
        pthread_mutex_lock(&client_count_mutex);
        if (client_count >= MAX_CLIENTS) {
            pthread_mutex_unlock(&client_count_mutex);
            sleep(1);
            continue;
        }
        pthread_mutex_unlock(&client_count_mutex);
        
        // Aloca memória para o novo cliente
        client_t *client = malloc(sizeof(client_t));
        if (!client) {
            perror("Falha ao alocar memória para o cliente");
            continue;
        }
        
        // Aceita a conexão do cliente
        client->socket_fd = accept(server_fd, (struct sockaddr *)&client->address,
                                 (socklen_t*)&addrlen);
        
        if (client->socket_fd < 0) {
            perror("Falha no accept");
            free(client);
            continue;
        }
        
        // Incrementa o contador de clientes e atribui ID
        pthread_mutex_lock(&client_count_mutex);
        client_count++;
        client->id = client_count;
        pthread_mutex_unlock(&client_count_mutex);
        
        // Cria thread para lidar com o cliente
        if (pthread_create(&threads[client_count-1], NULL, handle_client, (void*)client) != 0) {
            perror("Falha ao criar thread");
            close(client->socket_fd);
            free(client);
            
            pthread_mutex_lock(&client_count_mutex);
            client_count--;
            pthread_mutex_unlock(&client_count_mutex);
            
            continue;
        }
        
        // Desvincula a thread para que seus recursos sejam liberados automaticamente
        pthread_detach(threads[client_count-1]);
    }
    
    // Fecha o socket do servidor e o logger
    close(server_fd);
    close_logger();
    
    return 0;
}
