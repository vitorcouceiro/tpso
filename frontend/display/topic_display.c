#include "topic_display.h"
#include "../../utils/includes.h"
#include "../../backend/models/comunicacao.h"

void displayTopics(Comunicacao comunicacao) {
    if (comunicacao.n_topics == 0) {
        printf(NO_TOPICS);
    } else {
        for (int i = 0; i < comunicacao.n_topics; i++) {
            printf("Topic: %s\n", comunicacao.topic[i].nome);
            printf("Numero de mensagens persistentes: %d\n", comunicacao.topic[i].n_persistentes);
            //printf("Estado: %s\n", comunicacao.topic[i].estado);
            printf("\n");
        }
    }
}