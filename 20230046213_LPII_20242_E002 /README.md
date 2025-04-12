# Spinlock Cache Implementation

Este projeto implementa um sistema de cache compartilhado entre múltiplas threads, utilizando spinlock para garantir exclusão mútua no acesso à seção crítica.

## Descrição

O programa demonstra uma solução para o problema da seção crítica usando spinlock com a operação atômica test-and-set fornecida pela GNU. O cache armazena pares de chave-valor que são acessados de forma segura por múltiplas threads concorrentes.

Características principais:
- Implementação de spinlock usando `__sync_lock_test_and_set` da GNU
- Sistema de cache com operações de leitura e escrita protegidas por spinlock
- Demonstração com múltiplas threads realizando operações concorrentes
- Proporção de 70% leituras e 30% escritas
- Medição precisa de tempo de execução e cálculo de métricas de desempenho

## Estrutura do Código

O código está organizado da seguinte forma:
- Implementação de spinlock (inicialização, lock e unlock)
- Funções de cache (inicialização, leitura e escrita)
- Função de thread para realizar operações concorrentes no cache
- Função principal que cria threads, mede o tempo de execução e calcula estatísticas

## Compilação e Execução

### Compilação

Para compilar o código, utilize o seguinte comando:

```bash
gcc -o spinlock_cache assignment.c -lpthread -O2
```

Em sistemas Unix mais antigos, pode ser necessário incluir a biblioteca de tempo real:

```bash
gcc -o spinlock_cache assignment.c -lpthread -lrt -O2
```

### Execução

Para executar o programa, utilize:

```bash
./spinlock_cache
```

## Saída Esperada

Ao executar o programa, você verá uma saída semelhante a esta:

```
Iniciando 4 threads para realizar operações no cache...
Thread 0: 7023 leituras, 2977 escritas
Thread 1: 6987 leituras, 3013 escritas
Thread 2: 7002 leituras, 2998 escritas
Thread 3: 6951 leituras, 3049 escritas

--- Estatísticas de Execução ---
Tempo de execução: 0.827 segundos
Total de leituras: 27963
Total de escritas: 12037
Total de operações: 40000
Operações por segundo: 48367.59
```

A saída mostra:
1. O número de operações de leitura e escrita realizadas por cada thread
2. O tempo total de execução
3. O número total de operações realizadas
4. A taxa de operações por segundo

Os números exatos podem variar dependendo do sistema e da execução, mas a proporção entre leituras e escritas deve ser aproximadamente 70:30.
