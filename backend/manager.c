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
    feed_fd = open(comunicacao.user.FEED_PIPE, O_WRONLY);

    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    write(feed_fd, &comunicacao, sizeof(Comunicacao));
    close(feed_fd);
}

/*Comunicacao receiveMsg(){
    int manager_fd;
    Comunicacao comunicacao;
    memset(&comunicacao, 0, sizeof(Comunicacao)); // Inicializa a estrutura

    manager_fd = open(MANAGER_PIPE, O_RDONLY);
    if (manager_fd == -1) {
        perror(ERROR_OPENING_MANAGER_PIPE);
        exit(EXIT_FAILURE);
    }

    read(manager_fd, &comunicacao, sizeof(Comunicacao));

    close(manager_fd);
    return comunicacao;
}*/

int countWords(char *buffer){
    int spaces = 0;

    for(int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == ' '){
            spaces++;
        }
    }
    
    return spaces + 1;
}

void showTopics(Comunicacao comunicacao,TDATA *td) {
    comunicacao.n_topics = td->n_topics;
    for(int i = 0; i < td->n_topics;i++){
        strcpy(comunicacao.topic[i].nome,td->topic[i].nome);
        comunicacao.topic[i].n_persistentes = td->topic[i].n_persistentes;
        strcpy(comunicacao.topic[i].estado,td->topic[i].estado);
    }
    strcpy(comunicacao.tipoInformacao,TOPICS);
    sendMsg(comunicacao);
}

void writeMsg(Comunicacao comunicacao,TDATA *td){
    int index = -1;

    char *command = strtok(comunicacao.buffer, SPACE);
    char *topic = strtok(NULL, SPACE);
    char *duration = strtok(NULL, SPACE);
    char *message = strtok(NULL, "");

    for(int i = 0; i < td->n_topics;i++){
        if(strcmp(topic, td->topic[i].nome) == 0){
            index = i;
            break;
        }
    }

    if(index == -1){
        if(td->n_topics >= MAX_TOPICS){
            strcpy(comunicacao.buffer,"Maximo de topicos atingido\n");
            sendMsg(comunicacao);
            return;
        }else{
            strcpy(td->topic[td->n_topics].nome,topic);
            td->topic[td->n_topics].n_persistentes = 0;
            strcpy(td->topic[td->n_topics].estado,"desbloqueado");
            td->n_topics ++;
         }
    }


    if(duration == 0){// mensagem nao persistente

    }else{ //mensagem persistente

    }
    sendMsg(comunicacao);
}

void listUsers(TDATA *td){
    if(td->n_users == 0){
        printf("Nenhum user conectado ate ao momento.\n");
    }else{
        printf("Users conectados:\n");
        for(int i = 0; i < td->n_users; i++){
            printf("- %s \n",td->user[i].nome);
        }
    }
}

void removeUser(TDATA *td, char *buffer) {
    char *username = strtok(NULL, SPACE);
    int user_found = 0;
    Comunicacao comunicacao;

    for (int i = 0; i < td->n_users; i++) {
        if (strcmp(td->user[i].nome, username) == 0) {
            user_found = 1;
            strcpy(comunicacao.user.FEED_PIPE,td->user[i].FEED_PIPE);
            strcpy(comunicacao.tipoInformacao,EXIT);
            for (int j = i; j < td->n_users - 1; j++) {
                strcpy(td->user[j].nome, td->user[j + 1].nome);
            }
            td->n_users--;
            printf("User %s removido com sucesso.\n", username);
            sendMsg(comunicacao);
            break;
        }else{
            strcpy(comunicacao.user.FEED_PIPE,td->user[i].FEED_PIPE);
            strcpy(comunicacao.tipoInformacao,EXIT_INFO);
            strcpy(comunicacao.buffer,td->user[i].nome);
            sendMsg(comunicacao);
        }
    }

    if (!user_found) {
        printf("User %s não encontrado.\n", username);
    }

    

}

void processCommandAdm(char *buffer, TDATA *td){
    int index = -1;
    int n_topics;
    char *command;


    if(buffer == NULL || strcmp(buffer, "") == 0){
        return;
    }

    n_topics = countWords(buffer);

    command = strtok(buffer,SPACE);

    for(int i = 0; i < N_COMMANDS_ADM; i++){
        if(strcmp(command,COMMANDS_ADM[i]) == 0){
            index = i;
        }
    }

    if(index == -1){
        printf(INVALID_COMMAND);
        return;
    }

    switch (index)
    {
        case 0:
            if(n_topics == 1){
                listUsers(td);
            }else{
                printf(SYNTAX_ERROR_USERS);
                return;
            }
            break;
        case 1:
            if(n_topics == 2){
                removeUser(td,buffer);
            }else{
                printf(SYNTAX_ERROR_REMOVE_USER);
                return;
            }
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
    int index = -1;
    char *command;
    TDATA *td = (TDATA *)ptdata;
    Comunicacao comunicacao;

    manager_fd = open(MANAGER_PIPE, O_RDWR);
    if (manager_fd == -1) {
        perror(ERROR_OPENING_MANAGER_PIPE);
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        read(manager_fd, &comunicacao, sizeof(Comunicacao));
        
        if (strcmp(comunicacao.tipoPedido, "login") == 0) {
            if (td->n_users == MAX_USERS) {
                strcpy(comunicacao.buffer, MAX_USERS_REACHED);
            } else {
                int user_exists = 0;
                for (int i = 0; i < td->n_users; i++) {
                    if (strcmp(td->user[i].nome, comunicacao.user.nome) == 0) {
                        user_exists = 1;
                        break;
                    }
                }
                if (user_exists) {
                    strcpy(comunicacao.buffer, USERNAME_ALREADY_EXISTS);
                } else {
                    // Adicionar user
                    strcpy(comunicacao.buffer, LOGIN_SUCCESS);
                    strcpy(td->user[td->n_users].nome, comunicacao.user.nome);
                    strcpy(td->user[td->n_users].FEED_PIPE, comunicacao.user.FEED_PIPE);
                    td->n_users++;
                }
            }
            sendMsg(comunicacao);
        } else if (strcmp(comunicacao.tipoPedido, "logout") == 0) {
            for (int i = 0; i < td->n_users; i++) {
                if (strcmp(comunicacao.user.nome, td->user[i].nome) == 0) {
                    for (int j = i; j < td->n_users - 1; j++) {
                        strcpy(td->user[j].nome, td->user[j + 1].nome);
                    }
                    td->n_users--;
                }
            }
        } else if (strcmp(comunicacao.tipoPedido, "linha_commands") == 0) {
            command = strtok(comunicacao.buffer, SPACE);
            strcpy(comunicacao.command, command);

            for (int i = 0; i < N_COMMANDS_USER; i++) {
                if (strcmp(command, COMMANDS_USER[i]) == 0) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                strcpy(comunicacao.buffer, INVALID_COMMAND);
                sendMsg(comunicacao);
                continue;
            }

            switch (index) {
                case 0: // TOPICS
                    showTopics(comunicacao, td);
                    break;
                case 1: // MSG
                    writeMsg(comunicacao, td);
                    break;
                case 2: // SUBSCRIBE
                    // subscribeTopics();
                    break;
                case 3: // UNSUBSCRIBE
                    // unsubscribeTopics();
                    break;
                case 4: // HELP
                    // helpUser();
                    break;
                default:
                    // sendMsg("Comando inválido\n");
                    continue;
            }

            if (strcmp(comunicacao.buffer, EXIT) == 0) {
                printf("A terminar o manager\n");
                break;
            }

        }
    }
    close(manager_fd);
    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_t thread;
    char buffer[MAX_MSG_SIZE];
    char *command;

    TDATA td;
    td.n_topics = 0;
    td.n_users = 0;
    td.topic->n_persistentes = 0;

    system("clear"); 
    
    signal(SIGINT, cleanup);

    if (argc != 1) {
        printf(INVALID_ARGS_MANA);
        exit(EXIT_FAILURE);
    }

    if (access(MANAGER_PIPE, F_OK) == 0) {
        printf(MANAGER_ALREADY_RUNNING);
        exit(EXIT_FAILURE);
    }

    if (mkfifo(MANAGER_PIPE, 0660) == -1) {
        perror("Erro ao criar MANAGER_PIPE");
        exit(EXIT_FAILURE);
    }
    
    if (access(MANAGER_PIPE, F_OK) != 0) {
        printf(MANAGER_NOT_RUNNING);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread, NULL, process_orders, (void*)&td) != 0) {
        perror(ERROR_CREATING_THREAD);
        return 1;
    }

    do {
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

        processCommandAdm(buffer, &td);

    } while (strcmp(buffer, CLOSE) != 0);

    unlink(MANAGER_PIPE);
    return 0;
}