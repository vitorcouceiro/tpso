#include "monitor_server_thread.h"
#include "../../utils/includes.h"
#include "../../utils/globals.h"

void *monitorServer(void *arg){
    char *FEED_PIPE = (char *)arg;
    while(1){
        if(access(MANAGER_PIPE,F_OK) != 0){
            system("clear");
            printf(MANAGER_SHUTDOWN);
            unlink(FEED_PIPE);
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
}
