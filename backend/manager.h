#ifndef MANAGER_H
#define MANAGER_H
#include "../utils/models/topic.h"
#include "../utils/models/user.h"
//#include <pthread.h>

//extern pthread_mutex_t td_mutex;

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