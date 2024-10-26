#ifndef MANAGER_H
#define MANAGER_H
#include "../backend/models/topic.h"
#include "../backend/models/user.h"

typedef struct 
{
    int n_user;
    int n_topics;
    Topic *topics;
    User *users;  
} TDATA;

#endif