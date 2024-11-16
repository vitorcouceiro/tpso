#ifndef TOPIC_H
#define TOPIC_H

#include "msg.h"
#include "subscriber.h"

typedef struct
{
    char nome[20];
    int n_subscribers;
    Subscriber subscribers[10];
    int n_persistentes;
    Msg persistente[5];
    int isLocked; // 0 - unlocked, 1 - locked
} Topic;

#endif // TOPIC_H
