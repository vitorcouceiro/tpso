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
            ResponseInfoError responseInfoError;
            user_found = 1;
            strcpy(responseInfoError.base.FEED_PIPE,td->user[i].FEED_PIPE);


            for (int j = i; j < td->n_users - 1; j++) {
                strcpy(td->user[j].nome, td->user[j + 1].nome);
                strcpy(td->user[j].FEED_PIPE, td->user[j + 1].FEED_PIPE);
            }

            td->n_users--;

            for (int i = 0; i < td->n_topics; i++) {
                for (int j = 0; j < td->topic[i].n_subscribers; j++) {
                    if (strcmp(td->topic[i].subscribers[j].nome, username) == 0) {
                        for (int k = j; k < td->topic[i].n_subscribers - 1; k++) {
                            strcpy(td->topic[i].subscribers[k].nome, td->topic[i].subscribers[k + 1].nome);
                        }
                        td->topic[i].n_subscribers--;
                        break;
                    }
                }
            }

            strcpy(responseInfoError.buffer,USER_REMOVED);
            responseInfoError.type = USER_EXPELLED;
            unicastInfoError(responseInfoError);

            if(td->n_users != 0){
                snprintf(responseInfoError.buffer, sizeof(responseInfoError.buffer), USER_REMOVED_SPECIFIC, username);
                responseInfoError.type = USER_EXPELLED_NOTIFICATION;
                broadcastUserExpelled(td,responseInfoError);
            }

            printf(USER_REMOVED_SUCCESS, username);
            break;
        }
    }

    if (!user_found) {
        printf(USER_NOT_FOUND, username);
    }
}

void handleLoginRequest(int manager_fd, TDATA *td) {
    ResponseInfoError responseInfoError;

    RequestAuthManager request;
    read(manager_fd, &request, sizeof(RequestAuthManager));
    strcpy(responseInfoError.base.FEED_PIPE, request.base.FEED_PIPE);

    if (td->n_users == MAX_USERS) {
        strcpy(responseInfoError.buffer, MAX_USERS_REACHED);
        responseInfoError.type = LOGIN_ERROR;
        unicastInfoError(responseInfoError);
    } else {
        int user_exists = 0;
        for (int i = 0; i < td->n_users; i++) {
            if (strcmp(td->user[i].nome, request.base.userName) == 0) {
                user_exists = 1;
                break;
            }
        }
        if (user_exists == 1) {
            strcpy(responseInfoError.buffer, USERNAME_ALREADY_EXISTS);
            responseInfoError.type = LOGIN_ERROR;
            unicastInfoError(responseInfoError);
        } else {
            // Adicionar user
            strcpy(responseInfoError.buffer, LOGIN_SUCCESSS);
            strcpy(td->user[td->n_users].nome, request.base.userName);
            strcpy(td->user[td->n_users].FEED_PIPE, request.base.FEED_PIPE);
            td->n_users++;
        }

    }
    responseInfoError.type = LOGIN_SUCCESS;
    unicastInfoError(responseInfoError);
}

void handleLogoutRequest(int manager_fd, TDATA *td) {
    RequestAuthManager request;
    read(manager_fd, &request, sizeof(RequestAuthManager));

    for (int i = 0; i < td->n_users; i++) {
        if (strcmp(request.base.userName, td->user[i].nome) == 0) {
            for (int j = i; j < td->n_users - 1; j++) {
                strcpy(td->user[j].nome, td->user[j + 1].nome);
            }
            td->n_users--;
            break;
        }
    }
}