#ifndef FEED_H
#define FEED_H

typedef struct
{
    int continua;
    char FEED_PIPE[256];
    pthread_mutex_t *ptrinco1;
    pthread_cond_t *cond;
} TFEED;

#endif
