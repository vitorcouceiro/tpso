#include "../communication/broadcast_communication.c"

void expelUser(TDATA *td, char *buffer) {
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