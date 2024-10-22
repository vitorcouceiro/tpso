#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "manager.h"
#include "../backend/models/topic.h"
#include "../utils/Commands.h"
#include "../utils/Exceptions.h"
#include "../utils/Globals.h"
#include "../backend/models/user.h"

void showTopics(Topic topic[20]) {
    for (int i = 0; i < 20; i++) {
        if (strlen(topic[i].nome) > 0) { 
            printf("Topic %d: %s\n", i + 1, topic[i].nome);
        }
    }
}

int main(int argc, char *argv[]) {
    int manager_fd,feed_fd;
    int index;
    char buffer[MAX_MSG_SIZE];
    char *command;
    User user[10];
    Topic topic[20];
    
    

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
        
        command = strtok(buffer,SPACE);
        
        close(manager_fd);

        printf("Mensagem: %s\n",buffer);

        for(int i = 0; i < N_COMMANDS; i++){
            if(strcmp(command, COMMANDS[i]) == 0){
                index = i;
                break;
            }
        }

        switch (index)
        {
            case 0: // TOPICS
                if(strlen(topic[0].nome) == 0){
                    printf("Entrou \n");
                    showTopics(topic);
                }else{
                    printf("Nao exitem topicos\n");
                }
            
                break;
            case 1: // MSG
                
            case 2: // SUBSCRIBE

            case 3: // UNSUBSCRIBE

            case 4: // HELP
                //helpUser();
            default:
                return 0;
        }


        if(strcmp(buffer,EXIT) == 0){
            printf("A terminar o manager\n");
            break;
        }

        //talvez fazer o enviar mensagem em um funcao a parte 
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

