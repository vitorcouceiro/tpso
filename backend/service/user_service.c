#include "user_service.h"
#include "../../utils/includes.h"
#include "../manager.h"
#include "../models/comunicacao.h"
#include "../communication/unicast_communication.h"
#include "../communication/broadcast_communication.h"
#include "../../utils/globals.h"

void expelUser(TDATA *td, char *buffer) {
    char *username = strtok(NULL, SPACE);
    int user_found = 0;

    for (int i = 0; i < td->n_users; i++) {
        if (strcmp(td->user[i].nome, username) == 0) {
            Comunicacao comunicacao;
            user_found = 1;
            strcpy(comunicacao.user.FEED_PIPE,td->user[i].FEED_PIPE);
            strcpy(comunicacao.tipoInformacao,EXIT);
            for (int j = i; j < td->n_users - 1; j++) {
                strcpy(td->user[j].nome, td->user[j + 1].nome);
            }
            td->n_users--;
            printf(USER_REMOVED_SUCCESS, username);
            unicastMsg(comunicacao);

            strcpy(comunicacao.tipoInformacao,EXIT_INFO);
            strcpy(comunicacao.buffer,td->user[i].nome);
            broadcastMsg(td,comunicacao);
            break;
        }
    }

    if (!user_found) {
        printf(USER_NOT_FOUND, username);
    }
}