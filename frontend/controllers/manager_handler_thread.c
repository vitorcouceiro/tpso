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
                    /*
                    printf("RECEBEU: %d\n",response.n_topics);
                    printf("RECEBEU: %s\n",response.topic[0].nome);
                    printf("RECEBEU: %s\n",response.topic[1].nome);
                    */
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
            case USER_EXPELLED_NOTIFICATION:
            case MSG_ERROR:
            case TOPIC_SUBSCRIBE:
            case MSG_CONFIRMATION:
                {
                    ResponseInfoError response;
                    read(feed_fd, &response, sizeof(ResponseInfoError));
                    printf("\n%s", response.buffer);
                }
                break;
            case MSG_NOTIFICATION:
                {
                    ResponseMsg response;
                    read(feed_fd, &response, sizeof(ResponseMsg));
                    printf("\n[MSG] %s %s %d %s\n",response.topicName,response.autorName,response.duration,response.message);
                }
                break;
            default:
                break;

        }
        printf("cmd > ");
        fflush(stdout);
    }
    close(feed_fd);
}