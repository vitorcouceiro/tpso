#include "topic_display.h"
#include "../../utils/includes.h"
#include "../../utils/models/comunicacao.h"

void displayTopics(ResponseListTopics responseListTopics) {
    if (responseListTopics.n_topics == 0) {
        printf(NO_TOPICS);
    } else {
        for (int i = 0; i < responseListTopics.n_topics; i++) {
            printf("Topic: %s\n", responseListTopics.topic[i].nome);
            printf("Numero de mensagens persistentes: %d\n", responseListTopics.topic[i].n_persistentes);
            //printf("Estado: %s\n", comunicacao.topic[i].estado);
            printf("\n");
        }
    }
}