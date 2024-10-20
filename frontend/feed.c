#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils/Commands.h"
#include "../utils/Exceptions.h"
#include "../utils/Globals.h"


int main (int argc, char *argv[]){
    int feed_fd,manager_fd;
    char buffer[MAX_MSG_SIZE];

    if(argc != 2){
        printf(INVALID_ARGS_FEED);
        exit(EXIT_FAILURE);
    }

    mkfifo(FEED_PIPE,0660);

    // argv[1] -> nome do user

    /*if(CONNECT_USER <= MAX_USERS){

    }else{
        printf(MAX_USERS_REACHED);
        exit(EXIT_FAILURE);
    }*/

    do{
        manager_fd = open(MANAGER_PIPE,O_WRONLY);

        if (manager_fd == -1) {
            perror("Erro ao abrir MANAGER_PIPE");
            exit(EXIT_FAILURE);
        }

        printf("cmd > ");
        fgets(buffer,MAX_MSG_SIZE,stdin);
        buffer[strlen(buffer)-1] = '\0';

        write(manager_fd,buffer,strlen(buffer)+1);
        close(manager_fd);

        if (strcmp(buffer,EXIT) == 0)
        {
            break;
        }
        
        feed_fd = open(FEED_PIPE, O_RDONLY);
        if (feed_fd == -1) {
            perror("Erro ao abrir FEED_PIPE");
            exit(EXIT_FAILURE);
        }

        read(feed_fd,buffer,MAX_MSG_SIZE);
        printf("Mensagem: %s\n",buffer);

        close(feed_fd);
        /*
            if(processCommand(command) == 0){

            }
        */
    }while (strcmp(buffer,EXIT) != 0);
    
    
    return 0;
}

int processCommand (char *command){

}
