#include "topic_service.h"
#include "../../utils/includes.h"
#include "../models/comunicacao.h"
#include "../manager.h"
#include "../../utils/globals.h"
#include "../communication/unicast_communication.h"

void sendTopics(int manager_fd, TDATA *td) {
    ResponseListTopics responseListTopics;

    RequestTopicsManager request;
    read(manager_fd, &request, sizeof(RequestTopicsManager));
    strcpy(responseListTopics.base.FEED_PIPE, request.base.FEED_PIPE);

    responseListTopics.n_topics = td->n_topics;
    for (int i = 0; i < td->n_topics; i++) {
        strcpy(responseListTopics.topic[i].nome, td->topic[i].nome);
        responseListTopics.topic[i].n_persistentes = td->topic[i].n_persistentes;
        responseListTopics.topic[i].isLocked = td->topic[i].isLocked;
    }
    responseListTopics.type = LIST_TOPICS;
    unicastListTopics(responseListTopics);
}

void lockTopic(TDATA *td, char *topic) {
    int index = -1;

    for (int i = 0; i < td->n_topics; i++) {
        if (strcmp(td->topic[i].nome, topic) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(TOPIC_NOT_FOUND, topic);
    } else {
        if (td->topic[index].isLocked == 1) {
            printf(TOPIC_ALREADY_LOCKED, topic);
        } else {
            td->topic[index].isLocked = 0;
            printf(TOPIC_LOCKED_SUCCESS, topic);
        }
    }
}

void unlockTopic(TDATA *td, char *topic) {
    int index = -1;

    for (int i = 0; i < td->n_topics; i++) {
        if (strcmp(td->topic[i].nome, topic) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(TOPIC_NOT_FOUND, topic);
    } else {
        if (td->topic[index].isLocked == 0) {
            printf(TOPIC_ALREADY_UNLOCKED, topic);
        } else {
            td->topic[index].isLocked = 1;
            printf(TOPIC_UNLOCKED_SUCCESS, topic);
        }
    }
}

void subscribeTopic(int manager_fd, TDATA *td) {
    RequestSubscribeUnsubscribeManager request;
    read(manager_fd, &request, sizeof(RequestSubscribeUnsubscribeManager));

    ResponseInfoError responseInfoError;
    strcpy(responseInfoError.base.FEED_PIPE, request.base.FEED_PIPE);
    int index = -1;

    for (int i = 0; i < td->n_topics; i++) {
        if (strcmp(td->topic[i].nome, request.topicName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {

        if (td->n_topics == MAX_TOPICS) {
            strcpy(responseInfoError.buffer, TOPIC_NOT_EXIST_MAX_REACHED);
            responseInfoError.type = TOPIC_SUBSCRIBE;
            unicastInfoError(responseInfoError);
            return;
        }


        strcpy(td->topic[td->n_topics].nome, request.topicName);
        td->topic[td->n_topics].n_persistentes = 0;
        td->topic[td->n_topics].isLocked = 0;
        strcpy(td->topic[td->n_topics].subscribers[0].nome, request.base.userName);
        td->topic[td->n_topics].n_subscribers = 1;
        td->n_topics++;

        strcpy(responseInfoError.buffer, SUBSCRIPTION_SUCCESS);
    } else {
        int userExists = 0;
        for (int j = 0; j < td->topic[index].n_subscribers; j++) {
            if (strcmp(td->topic[index].subscribers[j].nome, request.base.userName) == 0) {
                userExists = 1;
                break;
            }
        }

        if (userExists) {
            strcpy(responseInfoError.buffer, USER_ALREADY_SUBSCRIBED);
        } else {
            strcpy(td->topic[index].subscribers[td->topic[index].n_subscribers].nome, request.base.userName);
            td->topic[index].n_subscribers++;
            strcpy(responseInfoError.buffer, SUBSCRIPTION_SUCCESS);
        }
    }

    responseInfoError.type = TOPIC_SUBSCRIBE;
    unicastInfoError(responseInfoError);
}
