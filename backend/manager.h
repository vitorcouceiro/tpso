#ifndef MANAGER_H
#define MANAGER_H
#include "../backend/models/topic.h"
#include "../backend/models/user.h"

void showTopics(Topic topic[20], int n_topicos);
void sendMsg(char *buffer);

typedef struct 
{
    int n_user;
    int n_topics;
    Topic topics[20];
    User user[10];  
} TDATA;

#endif