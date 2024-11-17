#include "manager.h"
#include "../utils/includes.h"
#include "view/manager_view.h"
#include "../utils/globals.h"

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