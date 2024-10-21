#include "Commands.h"

//Globals
#define MAX_MSG_SIZE 300
#define MAX_USERS 10
#define N_COMMANDS 5
#define SPACE " "

char *COMMANDS[]={TOPICS,MSG,SUBCRIBE,UNSUBCRIBE,HELP}; 
int CONNECTED_USERS = 0;

//Const pipes
#define FEED_PIPE "../tmp/feed_pipe"
#define MANAGER_PIPE "../tmp/manager_pipe"