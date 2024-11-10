#include "manager_handler_thread.h"
#include "../../utils/includes.h"
#include "../../backend/models/comunicacao.h"
#include "../feed.h"
#include "../display/topic_display.h"

void *managerHandlerThread(void *ptdata) {
    const TFEED *td = (TFEED *)ptdata;


    int feed_fd = open(td->FEED_PIPE, O_RDWR);
    if (feed_fd == -1) {

        perror(ERROR_OPENING_FEED_PIPE);
        unlink(td->FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    while (1) {
        ResponseType responseType;
        read(feed_fd, &responseType, sizeof(ResponseType));

        //system("clear");

        switch (responseType) {
            case LIST_TOPICS:
                {
                    ResponseListTopics response;
                    read(feed_fd, &response, sizeof(ResponseListTopics));
                    displayTopics(response);
                }
                break;
            case USER_EXPELLED:
                {
                    ResponseInfoError response;
                    read(feed_fd, &response, sizeof(ResponseInfoError));
                    printf("\n%s", response.buffer);
                    exit(EXIT_FAILURE);
                }
                break;
            case USER_EXPELLED_NOTIFICATION:
            case MSG_ERROR:
            case MSG_CONFIRMATION:
                {
                    ResponseInfoError response;
                    read(feed_fd, &response, sizeof(ResponseInfoError));
                    printf("\n%s", response.buffer);
                }
                break;
            default:
                break;

        }

        /*
        if(strcmp(comunicacao.tipoInformacao,EXIT_INFO) == 0){
            displayExitInfo(comunicacao);
        }else if(strcmp(comunicacao.tipoInformacao, EXIT) == 0){
            unlink(td->FEED_PIPE);
            printf(USER_REMOVED);
        }*/

        printf("cmd > ");
        fflush(stdout);
    }
    close(feed_fd);
}