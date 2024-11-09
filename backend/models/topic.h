#ifndef TOPIC_H
#define TOPIC_H

#include "msg.h"

typedef struct
{
    char nome[20];
    int n_persistentes;
    //char estado[20];
    Msg persistente[5];
    int isLocked;
} Topic;

#endif // TOPIC_H
