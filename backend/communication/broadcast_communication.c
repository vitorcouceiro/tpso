#include "../../utils/includes.h"
#include "../manager.h"
#include "../models/comunicacao.h"
#include "../communication/unicast_communication.h"

void broadcastUserExpelled(const TDATA *td,ResponseInfoError responseInfoError) {
    for (int i = 0; i < td->n_users; i++) {
        printf("%s\n", td->user[i].FEED_PIPE);
        strcpy(responseInfoError.base.FEED_PIPE, td->user[i].FEED_PIPE);
        unicastInfoError(responseInfoError);
    }
}


void broadcastMsg(const TDATA *td, ResponseMsg responseMsg) {
    char feed_pipe [256];
    strcpy(feed_pipe, responseMsg.base.FEED_PIPE);

    for (int i = 0; i < td->n_users; i++) {
        if(strcmp(feed_pipe, td->user[i].FEED_PIPE) != 0) {
            //verificar se o user esta subscrito ao topico
            strcpy(responseMsg.base.FEED_PIPE, td->user[i].FEED_PIPE);
            unicastMsg(responseMsg);
        }
    }
}
