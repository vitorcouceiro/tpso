#include "msg_service.h"
#include "../../utils/includes.h"
#include "../communication/unicast_communication.h"
#include "../communication/broadcast_communication.h"
#include "../../utils/globals.h"

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

    if(index == -1) { // se o topico nao existir
        if(td->n_topics >= MAX_TOPICS){
            responseInfoError.type = MSG_ERROR;
            strcpy(responseInfoError.buffer, MAX_TOPICS_REACHED);
            unicastInfoError(responseInfoError);
            return;
        }

        if(request.duration == 0) {
            responseMsg.type = MSG_NOTIFICATION;
            strcpy(responseMsg.topicName, request.topicName);
            strcpy(responseMsg.message, request.message);
            broadcastMsg(td, responseMsg);
        }else {

        }

    }else {

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