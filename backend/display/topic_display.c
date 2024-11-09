#include "topic_display.h"
#include "../manager.h"
#include "../../utils/includes.h"

void displayTopics(TDATA *td){
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
