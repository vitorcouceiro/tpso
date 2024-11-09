#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_MSG_SIZE 300
#define MAX_USERS 10
#define MAX_TOPICS 20
#define MAX_PERSISTENT_MSG 5
#define N_COMMANDS_USER 5
#define N_COMMANDS_ADM 8

#define SPACE " "

extern char *COMMANDS_USER[];
extern char *COMMANDS_ADM[];
extern int CONNECTED_USERS;

#define MANAGER_PIPE "../tmp/manager_pipe"

#endif