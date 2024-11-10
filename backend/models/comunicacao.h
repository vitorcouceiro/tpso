#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "user.h"
#include "topic.h"
#include "msg.h"

typedef enum {
    LOGIN,
    LOGOUT,
    TOPICS,
    MSG,
    SUBSCRIBE,
    UNSUBSCRIBE
} RequestType;

typedef struct {
    char userName[20];
    char FEED_PIPE[256];
}Base;

typedef struct {
    RequestType type;
    Base base;
}RequestAuthManager; // pedido do manager (login/logout)

typedef struct {
    RequestType type;
    Base base;
    char topicName[50];
} RequestTopicsManager;


typedef struct {
    RequestType type;
    Base base;
    char topicName[50];
    int duration;
    char message[256];
} RequestMsgManager;

typedef struct {
    RequestType type;
    Base base;
    char topicName[50];
} RequestSubscribeUnsubscribeManager; // pedido do manager (subscribe/unsubscribe)

typedef enum {
    LIST_TOPICS,
    LOGIN_SUCCESS,
    LOGIN_ERROR,
    USER_EXPELLED,
    USER_EXPELLED_NOTIFICATION,


    MSG_CONFIRMATION,
    SUBSCRIBE_CONFIRMATION,
    UNSUBSCRIBE_CONFIRMATION,

    EXPEL_USER,
} ResponseType;

typedef struct {
    ResponseType type;
    Base base;
    Topic topic[20];
    int n_topics;
} ResponseListTopics;

typedef struct {
    ResponseType type;
    Base base;
    char buffer[300];
}ResponseInfoError;

typedef struct {
    ResponseType type;
    Base base;
    char topicName[50];
    char message[256];
} ResponseMsg;

typedef struct {
    ResponseType type;
}ResponseExpelUser;


#endif // COMUNICACAO_H
