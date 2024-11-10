#ifndef BROADCAST_COMMUNICATION_H
#define BROADCAST_COMMUNICATION_H
#include "../models/comunicacao.h"
#include "../manager.h"
//void broadcastMsg(TDATA *td, Comunicacao comunicacao);
void broadcastUserExpelled(const TDATA *td,ResponseInfoError responseInfoError);
#endif //BROADCAST_COMMUNICATION_H
