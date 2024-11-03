#ifndef FEED_H
#define FEED_H



typedef struct
{
    int feed_fd;
    int manager_fd;
    char FEED_PIPE[256];
} TFEED;


void *monitorServer(void *arg);
int countWords(char *buffer);
int processCommand(char *buffer);

#include "../backend/models/comunicacao.h"
#include "../utils/exceptions.h"
#include "../utils/globals.h"
#include "../frontend/view/feed_view.c"

#endif
