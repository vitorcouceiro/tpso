#ifndef PERMANENT_MSG_THREAD_H
#define PERMANENT_MSG_THREAD_H
#include "../manager.h"

#define CHECK_INTERVAL 1

void *permanentMsgHandlerThread(void *ptdata);
void removePersistentMsg(TDATA *td, int topicIndex, int msgIndex);

#endif //PERMANENT_MSG_THREAD_H