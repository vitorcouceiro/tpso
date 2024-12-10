#include "msg_service.h"
#include "../../utils/includes.h"
#include "../communication/unicast_communication.h"
#include "../communication/broadcast_communication.h"
#include "../../utils/globals.h"
#include "../manager.h"
#include <stdio.h>

void readtxt(char *filename, TDATA *td) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror(ERROR_OPENING_FILE);
        exit(EXIT_FAILURE);
    }

    char line[MAX_MSG_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char topicName[20];
        char userName[20];
        int duration;
        char message[MAX_MSG_SIZE];

        if (sscanf(line, "%s %s %d %[^\n]", topicName, userName, &duration, message) == 4) {

            int topicIndex = -1;
            for (int i = 0; i < td->n_topics; i++) {
                if (strcmp(td->topic[i].nome, topicName) == 0) {
                    topicIndex = i;
                    break;
                }
            }

            if (topicIndex == -1) {
                if (td->n_topics < MAX_TOPICS) {
                    strcpy(td->topic[td->n_topics].nome, topicName);
                    td->topic[td->n_topics].isLocked = 1;
                    strcpy(td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].autor, userName);
                    strcpy(td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].msg, message);
                    td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].duration = duration;
                    td->topic[td->n_topics].n_persistentes++;
                    td->n_topics++;
                }
            } else {
                if (td->topic[topicIndex].n_persistentes < MAX_PERSISTENT_MSG) {
                    strcpy(td->topic[topicIndex].persistente[td->topic[topicIndex].n_persistentes].autor, userName);
                    strcpy(td->topic[topicIndex].persistente[td->topic[topicIndex].n_persistentes].msg, message);
                    td->topic[topicIndex].persistente[td->topic[topicIndex].n_persistentes].duration = duration;
                    td->topic[topicIndex].n_persistentes++;
                }
            }
        }
    }

    fclose(file);
}

void createMsg(int manager_fd, TDATA *td) {

    RequestMsgManager request;
    read(manager_fd, &request, sizeof(RequestMsgManager));

    ResponseInfoError responseInfoError;
    strcpy(responseInfoError.base.FEED_PIPE, request.base.FEED_PIPE);

    ResponseMsg responseMsg;
    strcpy(responseMsg.base.FEED_PIPE, request.base.FEED_PIPE);

    int index = -1;
    for(int i = 0; i < td->n_topics;i++){
        if(strcmp(request.topicName, td->topic[i].nome) == 0){
            index = i;
            break;
        }
    }

    //alterar aqui codigo porque nao sei se ao enviar um topico a algo que nao existe se ele cria o topico

    if(index == -1) { // se o topico nao existir
        if(td->n_topics >= MAX_TOPICS){
            responseInfoError.type = MSG_ERROR;
            strcpy(responseInfoError.buffer, MAX_TOPICS_REACHED);
            unicastInfoError(responseInfoError);
            return;
        }

        if(request.duration == 0) {
            //nao sei ate que ponto faz sentido esta a escrever uma mensagem nao permanente que nao existe ou seja nao esta subscrito por ninguem
            /*
            responseMsg.type = MSG_NOTIFICATION;
            responseMsg.duration = request.duration;
            strcpy(responseMsg.topicName, request.topicName);
            strcpy(responseMsg.message, request.message);
            strcpy(responseMsg.autorName, request.base.userName);
            strcpy(responseMsg.base.FEED_PIPE, request.base.FEED_PIPE);
            broadcastMsg(td, responseMsg);


            responseInfoError.type = MSG_CONFIRMATION;
            strcpy(responseInfoError.base.FEED_PIPE, request.base.FEED_PIPE);
            strcpy(responseInfoError.buffer, MSG_SENT);
            unicastInfoError(responseInfoError);
            */
        }else{
            strcpy(td->topic[td->n_topics].nome, request.topicName);
            td->topic[td->n_topics].isLocked = 0;
            strcpy(td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].autor, request.base.userName);
            strcpy(td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].msg, request.message);
            td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].duration = request.duration;
            td->topic[td->n_topics].n_persistentes++;
            td->n_topics++;

            responseInfoError.type = MSG_CONFIRMATION;
            strcpy(responseInfoError.buffer, "Mensagem criada com sucesso\n");
            unicastInfoError(responseInfoError);

            responseMsg.type = MSG_NOTIFICATION;
            responseMsg.duration = request.duration;
            strcpy(responseMsg.topicName, request.topicName);
            strcpy(responseMsg.message, request.message);
            strcpy(responseMsg.autorName, request.base.userName);
            strcpy(responseMsg.base.FEED_PIPE, request.base.FEED_PIPE);
            broadcastMsg(td, responseMsg,index);
        }
    }else {
        if(td->topic[index].isLocked == 1) {
            responseInfoError.type = MSG_ERROR;
            strcpy(responseInfoError.buffer, TOPIC_LOCKED);
            unicastInfoError(responseInfoError);
        }else {
            if(request.duration == 0) {
                responseMsg.type = MSG_NOTIFICATION;
                responseMsg.duration = request.duration;
                strcpy(responseMsg.topicName, request.topicName);
                strcpy(responseMsg.message, request.message);
                strcpy(responseMsg.autorName, request.base.userName);
                strcpy(responseMsg.base.FEED_PIPE, request.base.FEED_PIPE);
                broadcastMsg(td, responseMsg,index);

                responseInfoError.type = MSG_CONFIRMATION;
                strcpy(responseInfoError.base.FEED_PIPE, request.base.FEED_PIPE);
                strcpy(responseInfoError.buffer, MSG_SENT);
                unicastInfoError(responseInfoError);
            }else {
                if(td->topic[index].n_persistentes == MAX_PERSISTENT_MSG){
                    responseInfoError.type = MSG_ERROR;
                    strcpy(responseInfoError.buffer, MAX_PERSISTENT_MSG_REACHED);
                    unicastInfoError(responseInfoError);
                }else{
                    strcpy(td->topic[index].persistente[td->topic[index].n_persistentes].autor, request.base.userName);
                    strcpy(td->topic[index].persistente[td->topic[index].n_persistentes].msg, request.message);
                    td->topic[index].persistente[td->topic[index].n_persistentes].duration = request.duration;
                    td->topic[index].n_persistentes++;

                    responseInfoError.type = MSG_CONFIRMATION;
                    strcpy(responseInfoError.buffer, "Mensagem criada com sucesso\n");
                    unicastInfoError(responseInfoError);

                    responseMsg.type = MSG_NOTIFICATION;
                    responseMsg.duration = request.duration;
                    strcpy(responseMsg.topicName, request.topicName);
                    strcpy(responseMsg.message, request.message);
                    strcpy(responseMsg.autorName, request.base.userName);
                    strcpy(responseMsg.base.FEED_PIPE, request.base.FEED_PIPE);
                    broadcastMsg(td, responseMsg,index);
                }
            }
        }
    }

}

void saveMessages(char filename[], TDATA *td){
    FILE *file = fopen(filename, "w");
    if(file == NULL){
        //nao sei como é que queres terminar o programa aqui
    }

    //se o n_topics for o numero de topicos que esta a ser usado no momento isto esta certo
    //se nao trocar por 20
    //also esta a assumir que os topicos que estao a ser usados estao todos juntos

    //por cada um dos topicos
    for (int i = 0; i< td->n_topics; ++i) {
        //por cada uma das mensagens persistentes dentro dos topicos
        for (int j = 0 ; j < td->topic[i].n_persistentes; ++j) {
            fprintf(file, "%s %s %d %s\n",td->topic[i].nome, td->topic[i].persistente[j].autor, td->topic[i].persistente[j].duration, td->topic[i].persistente[j].msg);
        }
    }

    fclose(file);
}
