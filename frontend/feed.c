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

int processCommand (char *buffer){
    int index = -1;
    int n_topics;
    char *command;
    command = strtok(buffer, SPACE);

    n_topics = countWords(buffer);

    for(int i = 0; i < N_COMMANDS; i++){
        if(strcmp(command, COMMANDS[i]) == 0){
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(INVALID_COMMAND);
        return 0;
    }

    switch (index)
    {
        case 0: // TOPICS
            if(n_topics == 1){
                return 1;
            }else{
                printf(SYNTAX_ERROR_TOPICS);
                return 0;
            }
        case 1: // MSG
            if(n_topics >= 4){
                char *topic = strtok(NULL, SPACE);
                char *duration = strtok(NULL, SPACE);
                char *message = strtok(NULL, "");

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return 0;
                }

                if (strlen(message) > 300) {
                    printf(MESSAGE_LENGTH_ERROR);
                    return 0;
                }

                return 1;
            }else{
                printf(SYNTAX_ERROR_MSG);
                return 0;
            }
        case 2: // SUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return 0;
                }

                return 1;
            }else{
                printf(SYNTAX_ERROR_SUBCRIBE);
                return 0;
            }
        case 3: // UNSUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return 0;
                }

                return 1;
            }else{
                printf(SYNTAX_ERROR_UNSUBCRIBE);
                return 0;
            }
        case 4: // HELP
            if(n_topics == 1){
                return 1;
            }else{
                printf(SYNTAX_ERROR_HELP);
                return 0;
            }
        default:
            return 0;
    }
}

int countWords(char *buffer){
    int spaces = 0;

    for(int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == ' '){
            spaces++;
        }
    }
    
    return spaces + 1;
}

void sendMsg(char *message){
        int manager_fd;
        manager_fd = open(MANAGER_PIPE,O_WRONLY);

        if (manager_fd == -1) {
            perror("Erro ao abrir MANAGER_PIPE");
            exit(EXIT_FAILURE);
        }

        write(manager_fd, message, strlen(message) + 1);
        close(manager_fd);
}

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
        sendMsg(argv[1]);
    }else{
        printf(MAX_USERS_REACHED);
        exit(EXIT_FAILURE);
    }*/

    do{
        printf("cmd > ");

        if (fgets(buffer, MAX_MSG_SIZE, stdin) == NULL) {
            printf(ERROR_READING_COMMAND);
            continue;
        }

        if (buffer[0] == '\n') {
            printf(EMPTY_COMMAND);
            continue;
        }

        buffer[strlen(buffer) - 1] = '\0';

        
        if(processCommand(buffer) == 1){
            
            sendMsg(buffer);

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

