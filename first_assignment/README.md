Cracking Passwords - Multi-process Approach

Este projeto implementa um sistema de quebra de senhas utilizando múltiplos processos em C. O programa criptografa e tenta descobrir senhas de 4 caracteres usando um método de força bruta.

🛠 Requisitos

Antes de compilar e executar, certifique-se de ter os seguintes requisitos instalados:

GCC (compilador de C)

Linux ou WSL (caso esteja no Windows)

Para instalar o GCC no Linux, execute:

sudo apt update && sudo apt install gcc -y

🚀 Compilação e Execução

1️⃣ Compilar o Código

Para compilar o código C, execute o seguinte comando no terminal:

gcc -o cracker cracker.c

Isso irá gerar um executável chamado cracker.

2️⃣ Criar as Pastas Necessárias

O código utiliza pastas específicas para armazenar as senhas criptografadas e as senhas descriptografadas. Certifique-se de criar as pastas antes de executar o programa:

mkdir -p passwords decrypted

3️⃣ Criar os Arquivos de Entrada

Adicione arquivos de texto na pasta passwords/ contendo senhas criptografadas. O código espera 10 arquivos nomeados de 001.txt a 010.txt. Cada arquivo deve conter uma senha criptografada por linha.

Exemplo:

echo "ABCD" > passwords/001.txt

4️⃣ Executar o Programa

Após a compilação, execute o programa com:

./cracker

O programa irá:

Criar 10 processos filhos.

Cada processo tentará descriptografar as senhas nos arquivos passwords/*.txt.

As senhas quebradas serão salvas na pasta decrypted/.

5️⃣ Verificar os Resultados

Após a execução, verifique os arquivos descriptografados na pasta decrypted/:

cat decrypted/001.txt

Isso mostrará a senha original correspondente à criptografia armazenada no arquivo passwords/001.txt.


