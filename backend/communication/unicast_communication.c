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
    close(feed_fd);
}