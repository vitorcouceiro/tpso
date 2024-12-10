#include "permanent_msg_thread.h"
#include "../../utils/includes.h"
#include "../manager.h"
#include <pthread.h>

void removePersistentMsg(TDATA *td, int topicIndex, int msgIndex) {
    for (int i = msgIndex; i < td->topic[topicIndex].n_persistentes - 1; i++) {
        td->topic[topicIndex].persistente[i] = td->topic[topicIndex].persistente[i + 1];
    }
    td->topic[topicIndex].n_persistentes--;
}


void *permanentMsgHandlerThread(void *ptdata) {
    TDATA *td = (TDATA *)ptdata;

    // ler ficheiro de mensagens persistentes

    while (1) {
        sleep(CHECK_INTERVAL);

        //meto aqui um mutex para que ele so elimine e mude o valor de mensagens se nao estiverem a ler ou a escrever mensagens
        pthread_mutex_lock(&td->mutex);
        for (int i = 0; i < td->n_topics; i++) {
            int j = 0;
            while (j < td->topic[i].n_persistentes) {
                if (td->topic[i].persistente[j].duration == 0) {
                    removePersistentMsg(td, i, j);
                } else {
                    td->topic[i].persistente[j].duration--;
                    j++;
                }
            }
        }

        pthread_mutex_unlock(&td->mutex);
    }
}
