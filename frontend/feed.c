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

pthread_mutex_t terminal_mutex;

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
            pthread_mutex_lock(&terminal_mutex);
            printf(ERROR_OPENING_MANAGER_PIPE);
            pthread_mutex_unlock(&terminal_mutex);
            unlink(FEED_PIPE);
            pthread_exit(NULL);
        }
        sleep(1);
    }
    return NULL;
}

int processCommand (char *buffer){
    int index = -1;
    int n_topics;
    char *command;

    if(strcmp(buffer,"") == 0){
        return 0;
    }

    command = strtok(buffer, SPACE);

    n_topics = countWords(buffer);

    for(int i = 0; i < N_COMMANDS_USER; i++){
        if(strcmp(command, COMMANDS_USER[i]) == 0){
            index = i;
            break;
        }
    }

    if (index == -1) {
        pthread_mutex_lock(&terminal_mutex);
        printf(INVALID_COMMAND);
        pthread_mutex_unlock(&terminal_mutex);
        return 0;
    }

    switch (index)
    {
        case 0: // TOPICS
            if(n_topics == 1){
                return 1;
            }else{
                pthread_mutex_lock(&terminal_mutex);
                printf(SYNTAX_ERROR_TOPICS);
                pthread_mutex_unlock(&terminal_mutex);
                return 0;
            }
        case 1: // MSG
            if(n_topics >= 4){
                char *topic = strtok(NULL, SPACE);
                char *duration = strtok(NULL, SPACE);
                char *message = strtok(NULL, "");

                if (strlen(topic) > 20) {
                    pthread_mutex_lock(&terminal_mutex);
                    printf(TOPIC_LENGTH_ERROR);
                    pthread_mutex_unlock(&terminal_mutex);
                    return 0;
                }

                if (strlen(message) > 300) {
                    pthread_mutex_lock(&terminal_mutex);
                    printf(MESSAGE_LENGTH_ERROR);
                    pthread_mutex_unlock(&terminal_mutex);
                    return 0;
                }

                return 1;
            }else{
                pthread_mutex_lock(&terminal_mutex);
                printf(SYNTAX_ERROR_MSG);
                pthread_mutex_unlock(&terminal_mutex);
                return 0;
            }
        case 2: // SUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    pthread_mutex_lock(&terminal_mutex);
                    printf(TOPIC_LENGTH_ERROR);
                    pthread_mutex_unlock(&terminal_mutex);
                    return 0;
                }

                return 1;
            }else{
                pthread_mutex_lock(&terminal_mutex);
                printf(SYNTAX_ERROR_SUBCRIBE);
                pthread_mutex_unlock(&terminal_mutex);
                return 0;
            }
        case 3: // UNSUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    pthread_mutex_lock(&terminal_mutex);
                    printf(TOPIC_LENGTH_ERROR);
                    pthread_mutex_unlock(&terminal_mutex);
                    return 0;
                }

                return 1;
            }else{
                pthread_mutex_lock(&terminal_mutex);
                printf(SYNTAX_ERROR_UNSUBCRIBE);
                pthread_mutex_unlock(&terminal_mutex);
                return 0;
            }
        case 4: // HELP
            if(n_topics == 1){
                return 1;
            }else{
                pthread_mutex_lock(&terminal_mutex);
                printf(SYNTAX_ERROR_HELP);
                pthread_mutex_unlock(&terminal_mutex);
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
        pthread_mutex_lock(&terminal_mutex);
        perror(ERROR_OPENING_MANAGER_PIPE);
        pthread_mutex_unlock(&terminal_mutex);
        exit(EXIT_FAILURE);
    }

    ssize_t n_bytes = write(manager_fd, &comunicacao, sizeof(Comunicacao));
    if (n_bytes != sizeof(Comunicacao)) {
        pthread_mutex_lock(&terminal_mutex);
        perror("Erro ao escrever no manager pipe");
        pthread_mutex_unlock(&terminal_mutex);
        close(manager_fd);
        exit(EXIT_FAILURE);
    }

    close(manager_fd);
}

Comunicacao receiveMsg(char *FEED_PIPE) {
    int feed_fd;
    Comunicacao comunicacao;
    memset(&comunicacao, 0, sizeof(Comunicacao)); // Inicializa a estrutura

    feed_fd = open(FEED_PIPE, O_RDONLY);
    if (feed_fd == -1) {
        pthread_mutex_lock(&terminal_mutex);
        perror(ERROR_OPENING_FEED_PIPE);
        pthread_mutex_unlock(&terminal_mutex);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    ssize_t n_bytes = read(feed_fd, &comunicacao, sizeof(Comunicacao));
    if (n_bytes == 0) {
        pthread_mutex_lock(&terminal_mutex);
        printf("Nenhuma mensagem para ler do feed pipe\n");
        pthread_mutex_unlock(&terminal_mutex);
        close(feed_fd);
        return comunicacao; // Retorna uma estrutura vazia ou inicializada
    } else if (n_bytes == -1) {
        pthread_mutex_lock(&terminal_mutex);
        perror("Erro ao ler feed pipe");
        pthread_mutex_unlock(&terminal_mutex);
        close(feed_fd);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    close(feed_fd);
    return comunicacao;
}

void *handleManagerResponse(void *ptdata) {
    TFEED *td = (TFEED *)ptdata;
    Comunicacao comunicacao;

    while (1) {
        comunicacao = receiveMsg(td->FEED_PIPE);
        
        pthread_mutex_lock(&terminal_mutex);
        if (strcmp(comunicacao.tipoInformacao, TOPICS) == 0) {
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
        } else if(strcmp(comunicacao.tipoInformacao, EXIT) == 0){
            unlink(td->FEED_PIPE);  
            printf(USER_REMOVED);
            pthread_mutex_unlock(&terminal_mutex);
            pthread_exit(NULL);              
        } else if(strcmp(comunicacao.tipoInformacao,EXIT_INFO) == 0){
            printf("O user %s foi expulso da plataforma\n",comunicacao.buffer);
        }
        pthread_mutex_unlock(&terminal_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int feed_fd, manager_fd;
    char buffer[MAX_MSG_SIZE];
    char FEED_PIPE[256];
    pthread_t monitor_thread, response_thread;        
    Comunicacao comunicacao;
    TFEED td;
    
    system("clear"); 

    if (pthread_mutex_init(&terminal_mutex, NULL) != 0) {
        perror("Erro ao iniciar o mutex do terminal");
        exit(EXIT_FAILURE);
    }

    if (argc != 2) {
        pthread_mutex_lock(&terminal_mutex);
        printf(INVALID_ARGS_FEED);
        pthread_mutex_unlock(&terminal_mutex);
        exit(EXIT_FAILURE);
    }

    if (access(MANAGER_PIPE, F_OK) != 0) {
        pthread_mutex_lock(&terminal_mutex);
        printf(MANAGER_NOT_RUNNING);
        pthread_mutex_unlock(&terminal_mutex);
        exit(EXIT_FAILURE);
    }

    snprintf(FEED_PIPE, sizeof(FEED_PIPE), "../tmp/pipe_%d", getpid());
    strcpy(td.FEED_PIPE, FEED_PIPE);

    mkfifo(FEED_PIPE, 0660);
    strcpy(comunicacao.user.FEED_PIPE, FEED_PIPE);

    if (pthread_create(&monitor_thread, NULL, monitorServer, (void *)FEED_PIPE) != 0) {
        pthread_mutex_lock(&terminal_mutex);
        perror(ERROR_CREATING_MONITOR_THREAD);
        pthread_mutex_unlock(&terminal_mutex);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&response_thread, NULL, handleManagerResponse, (void *)&td) != 0) {
        pthread_mutex_lock(&terminal_mutex);
        perror(ERROR_CREATING_RESPONSE_THREAD);
        pthread_mutex_unlock(&terminal_mutex);
        unlink(FEED_PIPE);  
        exit(EXIT_FAILURE);
    }

    strcpy(comunicacao.user.nome, argv[1]);
    strcpy(comunicacao.tipoPedido, "login");

    sendMsg(comunicacao);
    comunicacao = receiveMsg(FEED_PIPE);

    if (strcmp(comunicacao.buffer, LOGIN_SUCCESS) != 0) { 
        pthread_mutex_lock(&terminal_mutex);
        printf(comunicacao.buffer);
        pthread_mutex_unlock(&terminal_mutex);
        close(feed_fd);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&terminal_mutex);
    printf(comunicacao.buffer);
    pthread_mutex_unlock(&terminal_mutex);

    do {
        sleep(1);
        pthread_mutex_lock(&terminal_mutex);
        printf("cmd > ");
        fflush(stdout);
        pthread_mutex_unlock(&terminal_mutex);

        if (fgets(buffer, MAX_MSG_SIZE, stdin) == NULL) {
            pthread_mutex_lock(&terminal_mutex);
            printf(ERROR_READING_COMMAND);
            pthread_mutex_unlock(&terminal_mutex);
            continue;
        }

        buffer[strlen(buffer) - 1] = '\0';

        if (strcmp(buffer, EXIT) == 0) {
            strcpy(comunicacao.tipoPedido,"logout");
            sendMsg(comunicacao);
            break;
        }

        if (processCommand(buffer) == 1) {
            strcpy(comunicacao.buffer, buffer);
            sendMsg(comunicacao);   
        }

    } while (1);

    unlink(FEED_PIPE);
    pthread_mutex_destroy(&terminal_mutex);
    return 0;
}