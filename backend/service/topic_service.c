#include "topic_service.h"
#include "../../utils/includes.h"
#include "../models/comunicacao.h"
#include "../manager.h"
#include "../communication/unicast_communication.h"


void sendTopics(Comunicacao comunicacao, const TDATA *td) {
    comunicacao.n_topics = td->n_topics;
    for (int i = 0; i < td->n_topics; i++) {
        strcpy(comunicacao.topic[i].nome, td->topic[i].nome);
        comunicacao.topic[i].n_persistentes = td->topic[i].n_persistentes;
        //modificar de maneira a que so o user que bloqueou é que esteja bloqueado
        //strcpy(comunicacao.topic[i].estado, td->topic[i].estado);
    }
    strcpy(comunicacao.tipoInformacao, TOPICS);
    unicastMsg(comunicacao);
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

