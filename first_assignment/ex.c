#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define NUM_LETRAS 26   // Número de letras no alfabeto inglês
#define ASCII_A 65      // Valor ASCII da letra 'A'
#define TAMANHO_SENHA 4 // Tamanho fixo da senha

// Constantes utilizadas para criptografia da senha
const int COPRIMES[4] = {3, 5, 7, 11};
const int SHIFTS[4] = {7, 11, 13, 17};

/**
 * @brief Codifica uma string de senha de tamanho fixo (4 caracteres).
 *
 * @param str Ponteiro para a string original a ser codificada.
 * @return char* Ponteiro para a string criptografada alocada dinamicamente.
 */
char* encrypt_pass(const char* str) { 
    char* str_result = (char*) malloc(sizeof(char) * (TAMANHO_SENHA + 1));

    if (!str_result) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TAMANHO_SENHA; i++) {
        char c = str[i];

        if (c >= 'A' && c <= 'Z') {
            int val = c - ASCII_A;
            val = (val * COPRIMES[i] + SHIFTS[i]) % NUM_LETRAS;
            str_result[i] = val + ASCII_A;
        } 
        else {
            perror("Erro: String contém caracteres inválidos.");
            free(str_result);
            exit(EXIT_FAILURE);
        }
    }

    str_result[TAMANHO_SENHA] = '\0';
    return str_result;
}

/**
 * @brief Lê senhas criptografadas de um arquivo e tenta decodificá-las gerando todas as combinações possíveis.
 *
 * @param filename Nome do arquivo de entrada contendo as senhas criptografadas.
 * @param filename_out Nome do arquivo de saída onde as senhas descriptografadas serão salvas.
 */
void process_file(const char* filename, const char* filename_out) {

    // Criação das variáveis para manipulação de arquivos
    FILE *file = fopen(filename, "r");
    FILE *fileout = fopen(filename_out, "w");

    printf("Processo PID %d: Quebrando senhas de %s\n", getpid(), filename);

    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char str[5];
    char linha[5];

    // Verificação de todas as combinações possíveis de AAAA a ZZZZ
    while (fgets(linha, sizeof(linha), file)) { 
        for (char i= 0; i < NUM_LETRAS; i++) {
            for (char j = 0; j <NUM_LETRAS; j++) {
                for (char k = 0; k <NUM_LETRAS; k++) {
                    for (char w = 0; w <NUM_LETRAS; w++) {
                        str[0] = ASCII_A + i;
                        str[1] = ASCII_A + j;
                        str[2] = ASCII_A + k;
                        str[3] = ASCII_A + w;
                        str[4] = '\0';

                        char* encrypted_str = encrypt_pass(str);

                        // Se a senha criptografada for igual à senha no arquivo, escreve a senha original no arquivo de saída
                        if (strcmp(encrypted_str, linha) == 0) {
                            fprintf(fileout, "%s\n", str);
                        }

                        free(encrypted_str);
                    }
                }
            }
        }
    }

    fclose(file);
    fclose(fileout);

    printf("Processo PID %d: Senhas quebradas salvas em %s\n", getpid(), filename_out);
}

int main() {
    struct timeval start, end;
    double elapsed_time;

    // Início da contagem do tempo de execução
    gettimeofday(&start, NULL);

    printf("Gerando 10 processos para processar arquivos...\n");

    // Lista dos arquivos de entrada contendo senhas criptografadas
    const char* filenames[] = {
        "passwords/001.txt", "passwords/002.txt", "passwords/003.txt", "passwords/004.txt", "passwords/005.txt",
        "passwords/006.txt", "passwords/007.txt", "passwords/008.txt", "passwords/009.txt", "passwords/010.txt"
    };

    // Lista dos arquivos de saída para armazenar as senhas descriptografadas
    const char* filenames_out[] = {
        "decrypted/001.txt", "decrypted/002.txt", "decrypted/003.txt", "decrypted/004.txt", "decrypted/005.txt",
        "decrypted/006.txt", "decrypted/007.txt", "decrypted/008.txt", "decrypted/009.txt", "decrypted/010.txt"
    };

    int num_files = sizeof(filenames) / sizeof(filenames[0]);

    // Criação de processos filhos para processar cada arquivo em paralelo
    for (int i = 0; i < num_files; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Processo filho executa a função de quebra de senha
            process_file(filenames[i], filenames_out[i]);
            exit(0); // Encerra o processo filho após processar o arquivo
        } else if (pid < 0) {
            // Erro ao criar um processo filho
            perror("Erro ao criar processo filho");
            exit(EXIT_FAILURE);
        }
    }

    // O processo pai espera todos os processos filhos terminarem
    for (int i = 0; i < num_files; i++) {
        wait(NULL);
    }

    printf("[Fim dos processos descendentes]\n");

    // Fim da contagem do tempo de execução
    gettimeofday(&end, NULL);

    // Calcula o tempo total de execução em milissegundos
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Tempo total de execução: %.2f ms\n", elapsed_time * 1000);

    return 0;
}
