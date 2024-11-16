#ifndef BROADCAST_COMMUNICATION_H
#define BROADCAST_COMMUNICATION_H
#include "../../utils/models/comunicacao.h"
#include "../manager.h"
void broadcastUserExpelled(const TDATA *td,ResponseInfoError responseInfoError);
void broadcastMsg(const TDATA *td, ResponseMsg responseMsg,int topicIndex);
#endif //BROADCAST_COMMUNICATION_H
