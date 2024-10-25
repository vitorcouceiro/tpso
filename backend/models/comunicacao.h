#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "user.h"
#include "topic.h"
#include "msg.h"

typedef struct Comunicacao {
    User user;
    Topic topic;
    Msg msg;

}Comunicacao;

#endif // COMUNICACAO_H
