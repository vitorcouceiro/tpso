#include "../manager.h"
#include "../controllers/feed_handler_thread.c"
#include "../display/topic_display.c"
#include "../display/list_users_display.c"
#include "../display/per_msg_display.c"
#include "../service/user_service.c"
#include "../service/topic_service.c"

int countWords(char *buffer){
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
    int n_topics;
    char *command;


    if(buffer == NULL || strcmp(buffer, "") == 0){
        return;
    }

    n_topics = countWords(buffer);

    command = strtok(buffer,SPACE);

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
        default:
            break;
    }
}

int managerView(){
    pthread_t thread;
    char buffer[MAX_MSG_SIZE];
    char *command;

    TDATA td;
    td.n_topics = 0;
    td.n_users = 0;
    td.topic->n_persistentes = 0;

    system("clear");

    signal(SIGINT, cleanup);

    if (access(MANAGER_PIPE, F_OK) == 0) {
        printf(MANAGER_ALREADY_RUNNING);
        exit(EXIT_FAILURE);
    }

    if (mkfifo(MANAGER_PIPE, 0660) == -1) {
        perror(ERROR_CREATING_MANAGER_PIPE);
        exit(EXIT_FAILURE);
    }

    if (access(MANAGER_PIPE, F_OK) != 0) {
        printf(MANAGER_NOT_RUNNING);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread, NULL, feedHandlerThread, (void*)&td) != 0) {
        perror(ERROR_CREATING_THREAD);
        return 1;
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

        system("clear");
        processCommandAdm(buffer, &td);

    } while (1);

    unlink(MANAGER_PIPE);
    return 0;
}