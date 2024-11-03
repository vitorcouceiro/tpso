#ifndef USER_H
#define USER_H
#include "topic.h"

typedef struct 
{
    char nome[10];
    char FEED_PIPE[256]; 
    Topic topic[20];
} User;

#endif // USER_H
