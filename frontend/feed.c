#include <stdio.h>
#include "../utils/utils.h"
#include "../backend/models/user.c"
;

int main (int argc, char *argv[]){
    if(argc != 2){
        printf(INVALID_ARGS);
        return 0;
    }
    // argv[1] -> nome do user

    return 0;
}

