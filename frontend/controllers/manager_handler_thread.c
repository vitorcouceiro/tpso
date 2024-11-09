#include "manager_handler_thread.h"
#include "../../utils/includes.h"
#include "../../backend/models/comunicacao.h"
#include "../feed.h"
#include "../display/exit_info_display.h"
#include "../display/topic_display.h"

void *managerHandlerThread(void *ptdata) {
    const TFEED *td = (TFEED *)ptdata;
    Comunicacao comunicacao;


    int feed_fd = open(td->FEED_PIPE, O_RDWR);
    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        unlink(td->FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    while (1) {
        read(feed_fd, &comunicacao, sizeof(Comunicacao));

        //system("clear");

        if (strcmp(comunicacao.tipoInformacao, TOPICS) == 0) {
            displayTopics(comunicacao);
        }else if(strcmp(comunicacao.tipoInformacao,EXIT_INFO) == 0){
            displayExitInfo(comunicacao);
        }else if(strcmp(comunicacao.tipoInformacao, EXIT) == 0){
            unlink(td->FEED_PIPE);
            printf(USER_REMOVED);
            exit(EXIT_FAILURE);
        }

        printf("cmd > ");
        fflush(stdout);
    }
    close(feed_fd);
}