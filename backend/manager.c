#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../utils/Commands.h"
#include "../utils/Exceptions.h"
#include "../utils/Globals.h"
#include "../backend/models/user.c"

;

int main(int argc, char *argv[]) {
    int manager_fd,feed_fd;
    char buffer[MAX_MSG_SIZE];

    if(argc != 1){
        printf(INVALID_ARGS_MANA);
        exit(EXIT_FAILURE);
    }

    mkfifo(MANAGER_PIPE,0660);

    while(1){
        manager_fd = open(MANAGER_PIPE,O_RDONLY);
       
        if (manager_fd == -1) {
            perror("Erro ao abrir MANAGER_PIPE");
            exit(EXIT_FAILURE);
        }

        read(manager_fd,buffer,MAX_MSG_SIZE);
        printf("Mensagem: %s\n",buffer);

        close(manager_fd);

        if(strcmp(buffer,EXIT) == 0){
            printf("A terminar o manager\n");
            break;
        }

        feed_fd = open(FEED_PIPE,O_WRONLY);
        if (feed_fd == -1) {
            perror("Erro ao abrir FEED_PIPE");
            exit(EXIT_FAILURE);
        }

        strcpy(buffer,"...");
        
        write(feed_fd,buffer,strlen(buffer)+1);
        close(feed_fd);
    }

    return 0;
}
