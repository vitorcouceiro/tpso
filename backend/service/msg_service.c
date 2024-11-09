#include "msg_service.h"
#include "../../utils/includes.h"
#include "../communication/unicast_communication.h"
#include "../communication/broadcast_communication.h"
#include "../../utils/globals.h"

void createMsg(Comunicacao comunicacao,TDATA *td){
    int index = -1;
    long time = 0;

    strtok(comunicacao.buffer, SPACE);

    char *topic = strtok(NULL, SPACE);
    if(topic == NULL){
        strcpy(comunicacao.buffer,SYNTAX_ERROR_MSG);
        unicastMsg(comunicacao);
        return;
    }

    char *duration = strtok(NULL, SPACE);
    if(duration == NULL){
        strcpy(comunicacao.buffer,SYNTAX_ERROR_MSG);
        unicastMsg(comunicacao);
        return;
    }

    char *message = strtok(NULL, "");
    if(message == NULL){
        strcpy(comunicacao.buffer,SYNTAX_ERROR_MSG);
        unicastMsg(comunicacao);
        return;
    }

    for(int i = 0; i < td->n_topics;i++){
        if(strcmp(topic, td->topic[i].nome) == 0){
            index = i;
            break;
        }
    }

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
