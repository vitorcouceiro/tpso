#include "unicast_communication.h"
#include "../../utils/includes.h"

void unicastInfoError(ResponseInfoError responseInfoError) {
    int feed_fd = open(responseInfoError.base.FEED_PIPE, O_WRONLY);

    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    write(feed_fd, &responseInfoError.type, sizeof(ResponseType));
    write(feed_fd, &responseInfoError, sizeof(ResponseInfoError));
    close(feed_fd);
}

void unicastListTopics(ResponseListTopics responseListTopics) {
    int feed_fd = open(responseListTopics.base.FEED_PIPE, O_WRONLY);

    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    write(feed_fd, &responseListTopics.type, sizeof(ResponseType));
    write(feed_fd, &responseListTopics, sizeof(ResponseListTopics));
    /*
    printf("ENVIADO: %d\n",responseListTopics.n_topics);
    printf("ENVIADO: %s\n",responseListTopics.topic[0].nome);
    printf("ENVIADO: %s\n",responseListTopics.topic[1].nome);
    */
    close(feed_fd);
}

void unicastMsg(ResponseMsg responseMsg) {
    int feed_fd = open(responseMsg.base.FEED_PIPE, O_WRONLY);

    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    write(feed_fd, &responseMsg.type, sizeof(ResponseType));
    write(feed_fd, &responseMsg, sizeof(ResponseMsg));
    close(feed_fd);
}

void unicastSubscribe(ResponseSubscribeTopic responseSubscribeTopic) {
    int feed_fd = open(responseSubscribeTopic.base.FEED_PIPE, O_WRONLY);

    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    write(feed_fd, &responseSubscribeTopic.type, sizeof(ResponseType));
    write(feed_fd, &responseSubscribeTopic, sizeof(ResponseSubscribeTopic));
    close(feed_fd);
}