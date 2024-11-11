#include "manager_view.h"
#include "../../utils/includes.h"
#include "../manager.h"
#include "../display/list_users_display.h"
#include "../display/per_msg_display.h"
#include "../display/topic_display.h"
#include "../service/user_service.h"
#include "../service/topic_service.h"
#include "../service/msg_service.h"
#include "../controllers/feed_handler_thread.h"
#include "../controllers/permanent_msg_thread.h"
#include "../../utils/globals.h"

int countWords(const char *buffer){
    int spaces = 0;
    for(int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == ' '){
            spaces++;
        }
    }

    return spaces + 1;
}

void processCommandAdm(char *buffer, TDATA *td){
    int index = -1;


    if(buffer == NULL || strcmp(buffer, "") == 0){
        return;
    }

    int n_topics = countWords(buffer);

    char *command = strtok(buffer,SPACE);

    for(int i = 0; i < N_COMMANDS_ADM; i++){
        if(strcmp(command,COMMANDS_ADM[i]) == 0){
            index = i;
        }
    }

    if(index == -1){
        printf(INVALID_COMMAND);
        return;
    }

    switch (index)
    {
        case 0:
            if(n_topics == 1){
                displayListUsers(td);
            }else{
                printf(SYNTAX_ERROR_USERS);
                return;
            }
            break;
        case 1:
            if(n_topics == 2){
                expelUser(td,buffer);
            }else{
                printf(SYNTAX_ERROR_REMOVE_USER);
                return;
            }
            break;
        case 2:
            if(n_topics == 1){
                displayTopics(td);
            }else{
                printf(SYNTAX_ERROR_TOPICS);
                return;
            }
            break;
        case 3:
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);
                displayPerMsg(td,topic);
            }else{
                printf(SYNTAX_ERROR_SHOW);
                return;
            }
            break;
        case 4:
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);
                lockTopic(td,topic);
            }else{
                printf(SYNTAX_ERROR_LOCK);
                return;
            }
            break;
        case 5:
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);
                unlockTopic(td,topic);
            }else{
                printf(SYNTAX_ERROR_UNLOCK);
                return;
            }
            break;
        case 6:
            if(n_topics == 1){
                printf(EXITING);
                unlink(MANAGER_PIPE);
                exit(EXIT_SUCCESS);
            }else{
                printf(SYNTAX_ERROR_CLOSE);
                return;
            }
            break;
        case 7:
            if(n_topics == 1){
                system("clear");
            }else{
                printf(SYNTAX_ERROR_CLEAR);
            }
            break;
        default:
            break;
    }
}

void managerView(){
    pthread_t threadfeed,threadpermanentmsgs;
    char buffer[MAX_MSG_SIZE];

    TDATA td;
    td.n_topics = 0;
    td.n_users = 0;
    td.topic->n_persistentes = 0;

    //system("clear");

    char *filename = getenv("MSG_FICH");
    if (filename == NULL) {
        printf("Variável de ambiente MSG_FICH não definida\n");
        exit(EXIT_FAILURE);
    }

    readtxt(filename, &td);

    signal(SIGINT, cleanup);

    if (access(MANAGER_PIPE, F_OK) == 0) {
        printf(MANAGER_ALREADY_RUNNING);
        exit(EXIT_FAILURE);
    }

    if (access("../tmp", F_OK) != 0) {
        if (mkdir("../tmp", 0770) == -1) {
            perror("Error creating tmp directory");
            exit(EXIT_FAILURE);
        }
    }

    if (mkfifo(MANAGER_PIPE, 0660) == -1) {
        perror(ERROR_CREATING_MANAGER_PIPE);
        exit(EXIT_FAILURE);
    }

    if (access(MANAGER_PIPE, F_OK) != 0) {
        printf(MANAGER_NOT_RUNNING);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&threadfeed, NULL, feedHandlerThread, (void*)&td) != 0) {
        perror(ERROR_CREATING_THREAD);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&threadpermanentmsgs, NULL, permanentMsgHandlerThread, (void*)&td) != 0) {
        perror(ERROR_CREATING_THREAD);
        exit(EXIT_FAILURE);
    }

    do {
        printf("cmd > ");

        if (fgets(buffer, MAX_MSG_SIZE, stdin) == NULL) {
            printf(ERROR_READING_COMMAND);
            continue;
        }

        if (buffer[0] == '\n') {
            printf(EMPTY_COMMAND);
            continue;
        }

        buffer[strlen(buffer) - 1] = '\0';

        //system("clear");
        processCommandAdm(buffer, &td);

    } while (1);

}