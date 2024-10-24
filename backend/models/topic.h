#ifndef TOPIC_H
#define TOPIC_H

#include "msg.h"

typedef struct
{
    char nome[20];
    Msg persistente[5];
    Msg n_persiste;
} Topic;

#endif // TOPIC_H
