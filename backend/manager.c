#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include "manager.h"
#include "../backend/models/topic.h"
#include "../utils/Commands.h"
#include "../utils/Exceptions.h"
#include "../utils/Globals.h"
#include "../backend/models/user.h"
#include "../backend/models/comunicacao.h"

void cleanup(int signo){
    unlink(MANAGER_PIPE);
    exit(EXIT_FAILURE);
}

void sendMsg(Comunicacao comunicacao){
    int feed_fd;
    feed_fd = open(comunicacao.FEED_PIPE,O_WRONLY);

    if (feed_fd == -1) {
        perror("Erro ao abrir FEED_PIPE");
        exit(EXIT_FAILURE);
    }

    write(feed_fd, &comunicacao, sizeof(Comunicacao));
    close(feed_fd);
}

Comunicacao receiveMsg(){
    int manager_fd;
    Comunicacao comunicacao;
    
    manager_fd = open(MANAGER_PIPE, O_RDONLY);
    if (manager_fd == -1) {
        perror("Erro ao abrir MANAGER_PIPE");
        exit(EXIT_FAILURE);
    }

    read(manager_fd, &comunicacao, sizeof(Comunicacao));
    close(manager_fd);

    return comunicacao;
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

void showTopics(Comunicacao comunicacao) {
    printf("entrou");
    strcpy(comunicacao.buffer,"ola");
    sendMsg(comunicacao);
}


void processCommandAdm(char *buffer){
    int index = -1;
    char *command;
    command = strtok(buffer,SPACE);

    int n_topics = countWords(buffer);

    for(int i = 0; i < N_COMMANDS_ADM; i++){
        if(strcmp(command,COMMANDS_ADM[i]) == 0){
            index = i;
        }
    }

    switch (index)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        break;
    }
}

void *process_orders(void *ptdata) {
    int manager_fd, feed_fd;
    int index;
    char *command;
    TDATA *td = (TDATA *)ptdata;
    Comunicacao comunicacao;

    while (1) {
        comunicacao = receiveMsg();

        if(strcmp(comunicacao.tipoPedido,"login") == 0){
            strcpy(comunicacao.buffer, "login_success");
            strcpy(comunicacao.tipoPedido, "linha_commands");
            sendMsg(comunicacao);
        }

        comunicacao = receiveMsg();

        command = strtok(comunicacao.buffer, SPACE);

        for (int i = 0; i < N_COMMANDS_USER; i++) {
            if (strcmp(command, COMMANDS_USER[i]) == 0) {
                index = i;
                break;
            }
        }

        switch (index) {
            case 0: // TOPICS
                showTopics(comunicacao);
                break;
            case 1: // MSG
                //writeMsg();
                break;
            case 2: // SUBSCRIBE
                //subscribeTopics();
                break;
            case 3: // UNSUBSCRIBE
                //unsubscribeTopics();
                break;
            case 4: // HELP
                //helpUser();
                break;
            default:
                //sendMsg("Comando inválido\n");
                continue;
        }

        if (strcmp(comunicacao.buffer, EXIT) == 0) {
            printf("A terminar o manager\n");
            break;
        }

        feed_fd = open(comunicacao.FEED_PIPE, O_WRONLY);
        if (feed_fd == -1) {
            perror("Erro ao abrir FEED_PIPE");
            exit(EXIT_FAILURE);
        }

        strcpy(comunicacao.buffer, "...");

        write(feed_fd, comunicacao.buffer, strlen(comunicacao.buffer) + 1);
        close(feed_fd);
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_t thread;
    char buffer[MAX_MSG_SIZE];
    char *command;

    TDATA td;

    signal(SIGINT,cleanup);

    if(argc != 1){
        printf(INVALID_ARGS_MANA);
        exit(EXIT_FAILURE);
    }

    if(access(MANAGER_PIPE,F_OK) == 0){
        printf(MANAGER_ALREADY_RUNNING);
        exit(EXIT_FAILURE);
    }

    mkfifo(MANAGER_PIPE,0660);
    
    if(access(MANAGER_PIPE,F_OK) != 0){
        printf(MANAGER_NOT_RUNNING);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread, NULL, process_orders, (void*)&td) != 0) {
        perror("Erro ao criar o thread");
        return 1;
    }

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

        processCommandAdm(buffer);

    }while(strcmp(buffer,CLOSE));

    unlink(MANAGER_PIPE);
    return 0;
}
