#ifndef TOPIC_SERVICE_H
#define TOPIC_SERVICE_H
#include "../manager.h"
#include "../models/comunicacao.h"
void sendTopics(Comunicacao comunicacao, const TDATA *td);
void lockTopic(TDATA *td, char *topic);
void unlockTopic(TDATA *td, char *topic);
#endif //TOPIC_SERVICE_H
