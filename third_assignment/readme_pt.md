# Servidor e Cliente Echo TCP

Este repositório contém um simples servidor e cliente Echo TCP com suporte para múltiplas conexões simultâneas.

## Funcionalidades

### Servidor
- Suporta múltiplas conexões de clientes simultâneas usando threads
- Limite de conexões configurável (padrão: 10 clientes)
- Sistema de log para registrar todas as mensagens e conexões
- Gerenciamento thread-safe de recursos compartilhados
- Aceita mensagens de clientes e envia-as de volta como eco

### Cliente
- Conecta-se a um servidor Echo TCP
- Suporta especificação do IP do servidor via linha de comando
- Interface de linha de comando simples para enviar mensagens
- Exibe as mensagens de eco recebidas do servidor

## Requisitos

- Sistema operacional Linux/Unix
- Compilador GCC
- Bibliotecas padrão de C
- Biblioteca POSIX Threads (pthread)

## Compilação

Para compilar o servidor e o cliente, execute os seguintes comandos:

```bash
# Compilar o servidor
gcc -o servidor base_server.c -lpthread

# Compilar o cliente
gcc -o cliente client.c
```

## Execução

### Servidor

Para iniciar o servidor na porta padrão (9090):

```bash
./servidor
```

O servidor criará um arquivo de log chamado `server_log.txt` no diretório atual.

### Cliente

Para iniciar o cliente e conectar ao servidor local (localhost):

```bash
./cliente
```

Para conectar a um servidor em um endereço IP específico:

```bash
./cliente 192.168.1.10
```

## Uso do Cliente

Após iniciar o cliente, você verá um prompt `>` onde poderá digitar mensagens para enviar ao servidor. Cada mensagem enviada será ecoada de volta pelo servidor.

Para encerrar o cliente, digite `EXIT` no prompt.

Exemplo:

```
Conectado ao servidor 127.0.0.1:9090
Digite mensagens para enviar (EXIT para sair):
> Olá mundo!
Eco: Olá mundo!
> Testando servidor echo
Eco: Testando servidor echo
> EXIT
Encerrando cliente...
Cliente encerrado
```

