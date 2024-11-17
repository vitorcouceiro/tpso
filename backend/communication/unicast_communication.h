#ifndef UNICAST_COMMUNICATION_H
#define UNICAST_COMMUNICATION_H
#include "../../utils/models/comunicacao.h"
void unicastInfoError(ResponseInfoError responseInfoError) ;
void unicastListTopics(ResponseListTopics responseListTopics);
void unicastMsg(ResponseMsg responseMsg);
void unicastSubscribe(ResponseSubscribeTopic responseSubscribeTopic);
#endif //UNICAST_COMMUNICATION_H
