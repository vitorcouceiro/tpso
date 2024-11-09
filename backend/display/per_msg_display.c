#include "per_msg_display.h"
#include "../../utils/includes.h"

void displayPerMsg(TDATA *td, char *topic) {
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
        for (int i = 0; i < td->topic[index].n_persistentes; i++) {
            printf("Mensagem %d: %s\n", i + 1, td->topic[index].persistente[i].msg);
        }
    }
}
