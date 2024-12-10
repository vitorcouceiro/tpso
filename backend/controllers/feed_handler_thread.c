#include "feed_handler_thread.h"
#include "../../utils/includes.h"
#include "../manager.h"
#include "../../utils/models/comunicacao.h"
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
                    //ponho aqui um mutex porque como ele manda o numero de mensagens persistentes no topico entao convem que ele nao mude esse valor enquanto ele esta a ler
                    pthread_mutex_lock(&td->mutex);
                    sendTopics(manager_fd, td);
                    pthread_mutex_unlock(&td->mutex);
                }
            break;
            case MSG:
                {
                    //ponho aqui um mutex para que ele nao tente adicionar uma mensagem enquanto ainda nao ha espaco ou enquanto o espaco esta a ser libertado do outro lado
                    pthread_mutex_lock(&td->mutex);
                    createMsg(manager_fd, td);
                    pthread_mutex_unlock(&td->mutex);
                }
            break;
            case SUBSCRIBE:
                {
                    //ponho aqui um mutex por nao convem que ele tente eliminar uma mensagem enquanto ela esta a ser enviada ao gajo que se acabou de subscrever
                    pthread_mutex_lock(&td->mutex);
                    subscribeTopic(manager_fd, td);
                    pthread_mutex_unlock(&td->mutex);
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
