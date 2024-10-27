#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include "feed.h"
#include "../backend/models/comunicacao.h"
#include "../utils/Commands.h"
#include "../utils/Exceptions.h"
#include "../utils/Globals.h"

int countWords(char *buffer){
    int spaces = 0;

    for(int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == ' '){
            spaces++;
        }
    }
    
    return spaces + 1;
}

void *monitorServer(void *arg){
    char *FEED_PIPE = (char *)arg;
    while(1){
        if(access(MANAGER_PIPE,F_OK) != 0){
            printf(ERROR_OPENING_MANAGER_PIPE);
            unlink(FEED_PIPE);
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    return NULL;
}

int processCommand (char *buffer){
    int index = -1;
    int n_topics;
    char *command;
    command = strtok(buffer, SPACE);

    n_topics = countWords(buffer);

    for(int i = 0; i < N_COMMANDS_USER; i++){
        if(strcmp(command, COMMANDS_USER[i]) == 0){
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

void sendMsg(Comunicacao comunicacao){
    
    int manager_fd;
    
    manager_fd = open(MANAGER_PIPE,O_WRONLY);
    if (manager_fd == -1) {
        perror(ERROR_OPENING_MANAGER_PIPE);
        exit(EXIT_FAILURE);
    }

    write(manager_fd, &comunicacao, sizeof(Comunicacao));
    close(manager_fd);
}

Comunicacao receiveMsg(char *FEED_PIPE){
    int feed_fd;
    Comunicacao comunicacao;
    
    feed_fd = open(FEED_PIPE, O_RDONLY);
    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    read(feed_fd, &comunicacao, sizeof(Comunicacao));
    close(feed_fd);

    return comunicacao;
}


void *handleManagerResponse(void *arg) {
    char *FEED_PIPE = (char *)arg;
    Comunicacao comunicacao;

    while (1) {
        comunicacao = receiveMsg(FEED_PIPE);
        if (strcmp(comunicacao.command, TOPICS) == 0) {
            if (comunicacao.n_topics == 0) {
                printf(NO_TOPICS);
            } else {
                for (int i = 0; i < comunicacao.n_topics; i++) {
                    printf("Topic: %s\n", comunicacao.topic[i].nome);
                    printf("Numero de mensagens persistentes: %d\n", comunicacao.topic[i].n_persistentes);
                    printf("Estado: %s\n", comunicacao.topic[i].estado);
                    printf("\n");
                }
            }
        } else if(strcmp(comunicacao.command, EXIT) == 0){
            
        }
    }
    return NULL;
}

int main (int argc, char *argv[]){
    int feed_fd,manager_fd;
    char buffer[MAX_MSG_SIZE];
    pthread_t monitor_thread, response_thread;
    Comunicacao comunicacao;
    pid_t pid = getpid();
    char FEED_PIPE[256];

    if(argc != 2){
        printf(INVALID_ARGS_FEED);
        exit(EXIT_FAILURE);
    }

    if(access(MANAGER_PIPE,F_OK) != 0){
        printf(MANAGER_NOT_RUNNING);
        exit(EXIT_FAILURE);
    }

    snprintf(FEED_PIPE, sizeof(FEED_PIPE), "../tmp/pipe_%d", pid);
    mkfifo(FEED_PIPE,0660);

    strcpy(comunicacao.user.FEED_PIPE,FEED_PIPE);

    if(pthread_create(&monitor_thread, NULL, monitorServer, (void *)FEED_PIPE) != 0){
        perror(ERROR_CREATING_MONITOR_THREAD);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&response_thread, NULL, handleManagerResponse, (void *)FEED_PIPE) != 0){
        perror(ERROR_CREATING_RESPONSE_THREAD);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    strcpy(comunicacao.user.nome,argv[1]);
    strcpy(comunicacao.tipoPedido,"login");
    
    sendMsg(comunicacao);
    comunicacao = receiveMsg(FEED_PIPE);

    if (strcmp(comunicacao.buffer,LOGIN_SUCCESS) != 0) { 
        printf(comunicacao.buffer);
        close(feed_fd);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }
    printf(comunicacao.buffer);

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

        if (strcmp(buffer, EXIT) == 0) {
            break;
        }

        if(processCommand(buffer) == 1){
            strcpy(comunicacao.buffer, buffer);
            sendMsg(comunicacao);
        }

    } while (1);

    unlink(FEED_PIPE);
    return 0;
}
