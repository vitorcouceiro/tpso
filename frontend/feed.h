#ifndef FEED_H
#define FEED_H


typedef struct
{
    int feed_fd;
    int manager_fd;
    char FEED_PIPE[256];
    char UserName[20];
} TFEED;


void *monitorServer(void *arg);

#endif
