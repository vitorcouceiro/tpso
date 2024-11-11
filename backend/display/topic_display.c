#include "topic_display.h"
#include "../manager.h"
#include "../../utils/includes.h"

void displayTopics(TDATA *td){

    printf("NOME1: %s\n", td->topic[0].nome);
    printf("NOME2: %s\n", td->topic[1].nome);
    printf("NOME3: %s\n", td->topic[2].nome);
    if (td->n_topics == 0) {
        printf(NO_TOPICS);
    } else {
        for (int i = 0; i < td->n_topics; i++) {
            printf("Topic: %s\n", td->topic[i].nome );
            printf("Numero de mensagens persistentes: %d\n",td->topic[i].n_persistentes);
            printf("\n");
        }
    }
}
