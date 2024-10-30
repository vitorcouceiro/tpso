#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "user.h"
#include "topic.h"
#include "msg.h"

typedef struct Comunicacao {
    User user;
    Topic topic[20];
    int n_topics;
    char buffer[300];
    char command[20];
    char tipoPedido[40];
    char tipoInformacao[50]; 
} Comunicacao;

#endif // COMUNICACAO_H
