#include "../../utils/includes.h"
#include "../manager.h"
#include "../models/comunicacao.h"
#include "../communication/unicast_communication.h"

void broadcastMsg(const TDATA *td, Comunicacao comunicacao) {
    char feed_pipe [256];
    strcpy(feed_pipe, comunicacao.user.FEED_PIPE);
    for (int i = 0; i < td->n_users; i++) {
        if(strcmp(feed_pipe, td->user[i].nome) != 0){
            strcpy(comunicacao.user.FEED_PIPE, td->user[i].FEED_PIPE);
            unicastMsg(comunicacao);
        }
    }
}
