#include "msg_service.h"
#include "../../utils/includes.h"
#include "../communication/unicast_communication.h"
#include "../communication/broadcast_communication.h"
#include "../../utils/globals.h"


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
                    td->topic[td->n_topics].isLocked = 0;
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

    ResponseMsg responseMsg;// isto esta mal
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
        }else{
            strcpy(td->topic[td->n_topics].nome, request.topicName);
            td->topic[td->n_topics].isLocked = 0;
            strcpy(td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].autor, request.base.userName);
            strcpy(td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].msg, request.message);
            td->topic[td->n_topics].persistente[td->topic[td->n_topics].n_persistentes].duration = request.duration;
            td->topic[td->n_topics].n_persistentes++;
            td->n_topics++;
        }
    }else {
        if(td->topic[index].isLocked == 1) { // topico bloqueado
            responseInfoError.type = MSG_ERROR;
            strcpy(responseInfoError.buffer, TOPIC_LOCKED);
            unicastInfoError(responseInfoError);
            return;
        }else {
            if(request.duration == 0) {
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
            }else {
                if(td->topic[index].n_persistentes == MAX_PERSISTENT_MSG){
                    responseInfoError.type = MSG_ERROR;
                    strcpy(responseInfoError.buffer, MAX_PERSISTENT_MSG_REACHED);
                    unicastInfoError(responseInfoError);
                    return;
                }else {
                    strcpy(td->topic[index].persistente[td->topic[index].n_persistentes].autor, request.base.userName);
                    strcpy(td->topic[index].persistente[td->topic[index].n_persistentes].msg, request.message);
                    td->topic[index].persistente[td->topic[index].n_persistentes].duration = request.duration;
                    td->topic[index].n_persistentes++;
                }
            }
        }
    }


}
/*
void createMsg(Comunicacao comunicacao,TDATA *td){


    // se o topico nao existir
    if(index == -1){
        // ALGO DE ERRADO AQUI
        if(td->n_topics >= MAX_TOPICS){
            strcpy(comunicacao.buffer,"Maximo de topicos atingido\n");
            unicastMsg(comunicacao);
            return;
        }
        if(atoi(duration) == 0) {
            strcpy(comunicacao.buffer,message);
            broadcastMsg(td,comunicacao);

        }else{
            if(td->topic[td->n_topics].n_persistentes == MAX_PERSISTENT_MSG){
                strcpy(comunicacao.buffer,"Maximo de mensagens persistentes atingido\n");
                unicastMsg(comunicacao);
                return;
            }
            strcpy(td->topic[td->n_topics].nome,topic);
            td->topic[td->n_topics].isLocked = 0;
            td->topic[td->n_topics].n_persistentes ++;

        }
        //strcpy(td->topic[td->n_topics].estado,"desbloqueado");
        td->n_topics ++;
    }else{
        if(td->topic[index].isLocked == 1){
            strcpy(comunicacao.buffer,"Topico bloqueado\n");
            unicastMsg(comunicacao);
            return;
        }else{
            if(duration == 0){// mensagem nao persistente

            }else{ //mensagem persistente

            }
        }

    }
    unicastMsg(comunicacao);
}
*/