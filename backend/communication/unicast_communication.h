#ifndef UNICAST_COMMUNICATION_H
#define UNICAST_COMMUNICATION_H
#include "../models/comunicacao.h"
void unicastInfoError(ResponseInfoError responseInfoError) ;
void unicastListTopics(ResponseListTopics responseListTopics);
void unicastMsg(ResponseMsg responseMsg);
#endif //UNICAST_COMMUNICATION_H
