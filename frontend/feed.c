#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils/Commands.h"
#include "../utils/Exceptions.h"
#include "../utils/Globals.h"


int main (int argc, char *argv[]){
    int fs;
    if(argc != 2){
        printf(INVALID_ARGS_FEED);
        exit(EXIT_FAILURE);
    }

    // argv[1] -> nome do user

    if(CONNECT_USER <= MAX_USERS){

    }else{
        printf(MAX_USERS_REACHED);
        exit(EXIT_FAILURE);
    }

    do{
        if(processCommand(command) == 0){

        }
    }while (strcmp(command,EXIT) != 0);
    
    
    return 0;
}

int processCommand (char *command){

}
