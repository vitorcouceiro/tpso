#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "feed.h"
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

        
        if(processCommand(buffer) != 0){
            write(manager_fd, buffer, strlen(buffer) + 1);
            close(manager_fd);

            if (strcmp(buffer, EXIT) == 0) {
                break;
            }

            feed_fd = open(FEED_PIPE, O_RDONLY);
            if (feed_fd == -1) {
                perror("Erro ao abrir FEED_PIPE");
                exit(EXIT_FAILURE);
            }

            read(feed_fd, buffer, MAX_MSG_SIZE);
            printf("Mensagem: %s\n", buffer);

            close(feed_fd);
        }

     }while (strcmp(buffer,EXIT) != 0);
    
    
    return 0;
}

int processCommand (char *buffer){
    int index;
    int n_topics;
    char *command;
    command = strtok(buffer,SPACE);

    printf("%d",command);

    n_topics = countWords(buffer);

    for(int i = 0; i < N_COMMANDS;i++){
        if(strcmp(command,COMMANDS[i])==0){
            index = i;
        }
    }

    switch (index)
    {
        case 0: //TOPICS
            if(n_topics == 1){

            }else{

            }
            break;
        case 1: //MSG
            if(n_topics >= 4){

            }else{

            }
            break;
        case 2: //SUBCRIBE
            if(n_topics == 2){

            }else{

            }
            break;
        case 3: //UNSUBCRIBE
            if(n_topics == 2){

            }else{

            }
            break;
        case 4: //HELP
            if(n_topics == 1){

            }else{

            }
            break;
        default:
            break;
    }
    return 0;
}

int countWords(char *buffer){
    int spaces = 0;

    for(int i = 0;buffer[i] != '\0';i++){
        if(buffer[i] == ' '){
            spaces++;
        }
    }
    
    return spaces + 1;
}