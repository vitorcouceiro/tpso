#ifndef MSG_SERVICE_H
#define MSG_SERVICE_H
#include "../manager.h"
#include "../models/comunicacao.h"
//void createMsg(Comunicacao comunicacao,TDATA *td);
void createMsg(int manager_fd, TDATA *td);
#endif //MSG_SERVICE_H
