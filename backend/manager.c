#include <fcntl.h>
#include "manager.h"

void cleanup(int signo){
    printf(EXITING);
    unlink(MANAGER_PIPE);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if(argc != 1){
        printf(INVALID_ARGS_MANA);
        exit(EXIT_FAILURE);
    }
    managerView();
}