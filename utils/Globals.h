#include "Commands.h"

//Globals
#define MAX_MSG_SIZE 300
#define MAX_USERS 10
#define MAX_TOPICS 20
#define N_COMMANDS_USER 4
#define N_COMMANDS_ADM 6

#define SPACE " "

char *COMMANDS_USER[]={TOPICS,MSG,SUBCRIBE,UNSUBCRIBE}; 
char *COMMANDS_ADM[]={USERS,REMOVE,TOPICS,SHOW,LOCK,UNLOCK};
int CONNECTED_USERS = 0;

//Const pipes
#define MANAGER_PIPE "../tmp/manager_pipe"
