#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../utils/utils.h"
#include "../backend/models/user.c"

;

int main(int argc, char *argv[]) {
    if(argc != 1){
        printf(INVALID_ARGS_MANA);
        exit(1);
    }

    return 0;
}
