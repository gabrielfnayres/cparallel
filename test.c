#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    
    printf("midiga o nome\n");
    scanf("%s", argv[1]);

    pid_t p = fork();


    if(p > 0){
        wait(NULL);
        return 0;
    } else if (p == 0){

        execl("./tt", "tt", argv[1], NULL);

    }


    return 0;
}