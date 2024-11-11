#include "feed_handler_thread.h"
#include "../../utils/includes.h"
#include "../manager.h"
#include "../models/comunicacao.h"
#include "../service/msg_service.h"
#include "../service/topic_service.h"
#include "../service/user_service.h"
#include "../../utils/globals.h"

void *feedHandlerThread(void *ptdata) {
    TDATA *td = (TDATA *)ptdata;

    int manager_fd = open(MANAGER_PIPE, O_RDWR);
    if (manager_fd == -1) {
        perror(ERROR_OPENING_MANAGER_PIPE);
        exit(EXIT_FAILURE);
    }

    while (1) {
        RequestType requestType;
        read(manager_fd, &requestType, sizeof(RequestType));
        switch (requestType) {
            case LOGIN:
                {
                    handleLoginRequest(manager_fd, td);
                }
            break;
            case LOGOUT:
                {
                    handleLogoutRequest(manager_fd, td);
                }
            break;
            case TOPICS:
                {
                    sendTopics(manager_fd, td);
                }
            break;
            case MSG:
                {
                    createMsg(manager_fd, td);
                }
            break;
            case SUBSCRIBE:
                {
                    subscribeTopic(manager_fd, td);
                }
            break;
            case UNSUBSCRIBE:
                {
                    unsubscribeTopic(manager_fd, td);
                }
            break;
            default:
                break;


        }
    }
}
