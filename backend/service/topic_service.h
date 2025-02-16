#ifndef TOPIC_SERVICE_H
#define TOPIC_SERVICE_H
#include "../manager.h"

void sendTopics(int manager_fd, TDATA *td);
void lockTopic(TDATA *td, char *topic);
void unlockTopic(TDATA *td, char *topic);
void subscribeTopic(int manager_fd,TDATA *td);
void unsubscribeTopic(int manager_fd, TDATA *td);
#endif //TOPIC_SERVICE_H
