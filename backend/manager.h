#ifndef MANAGER_H
#define MANAGER_H
#include "../backend/models/topic.h"
#include "../backend/models/user.h"

typedef struct 
{
    int n_users;
    int n_topics;
    Topic topic[20];
    User user[10];  
} TDATA;

#endif