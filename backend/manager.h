#ifndef MANAGER_H
#define MANAGER_H
#include "../backend/models/topic.h"
#include "../backend/models/user.h"



typedef struct
{
    int n_users;
    int n_topics;
    int manager_fd;
    Topic topic[20];
    User user[10];
} TDATA;

void cleanup(int signo);

#endif