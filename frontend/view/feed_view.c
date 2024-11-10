#include "feed_view.h"
#include "../../utils/includes.h"
#include "../../backend/models/comunicacao.h"
#include "../feed.h"
#include "../controllers/manager_handler_thread.h"
#include "../../utils/globals.h"

int countWords(char *buffer){
    int spaces = 0;
    for(int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == ' '){
            spaces++;
        }
    }

    return spaces + 1;
}

void processCommand (const char *buffer, TFEED td){
    int index = -1;

    if(strcmp(buffer,"") == 0){
        return;
    }

    char buffer_copy[MAX_MSG_SIZE];
    strcpy(buffer_copy, buffer);

    int n_topics = countWords(buffer_copy);
    char *command = strtok(buffer_copy, SPACE);

    for(int i = 0; i < N_COMMANDS_USER; i++){
        if(strcmp(command, COMMANDS_USER[i]) == 0){
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(INVALID_COMMAND);
        return ;
    }

    switch (index)
    {
        case 0: // TOPICS
            if(n_topics == 1){
                RequestTopicsManager requestTopicsManager;
                requestTopicsManager.type = TOPICS;
                strcpy(requestTopicsManager.base.userName, td.UserName);
                strcpy(requestTopicsManager.base.FEED_PIPE, td.FEED_PIPE);
                write(td.manager_fd , &requestTopicsManager.type, sizeof(RequestType));
                write(td.manager_fd, &requestTopicsManager, sizeof(RequestTopicsManager));
                return ;
            }else{
                printf(SYNTAX_ERROR_TOPICS);
                return ;
            }
        case 1: // MSG
            if(n_topics >= 4){
                const char *topic = strtok(NULL, SPACE);
                strtok(NULL, SPACE); // skip duration
                char *message = strtok(NULL, "");

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return ;
                }

                if (strlen(message) > 300) {
                    printf(MESSAGE_LENGTH_ERROR);
                    return ;
                }

                RequestMsgManager requestMsgManager;
                requestMsgManager.type = MSG;
                strcpy(requestMsgManager.topicName, topic);
                requestMsgManager.duration = atoi(strtok(NULL, SPACE));
                strcpy(requestMsgManager.message, message);
                strcpy(requestMsgManager.base.userName, td.UserName);
                strcpy(requestMsgManager.base.FEED_PIPE, td.FEED_PIPE);
                write(td.manager_fd, &requestMsgManager, sizeof(RequestMsgManager));

                return ;
            }else{
                printf(SYNTAX_ERROR_MSG);
                return ;
            }
        case 2: // SUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return ;
                }

                RequestSubscribeUnsubscribeManager requestSubscribe;
                requestSubscribe.type = SUBSCRIBE;
                strcpy(requestSubscribe.topicName, topic);
                strcpy(requestSubscribe.base.userName, td.UserName);
                strcpy(requestSubscribe.base.FEED_PIPE, td.FEED_PIPE);
                write(td.manager_fd, &requestSubscribe, sizeof(RequestSubscribeUnsubscribeManager));

                return ;
            }else{
                printf(SYNTAX_ERROR_SUBSCRIBE);
                return ;
            }
        case 3: // UNSUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return ;
                }

                RequestSubscribeUnsubscribeManager requestUnsubscribe;
                requestUnsubscribe.type = UNSUBSCRIBE;
                strcpy(requestUnsubscribe.topicName, topic);
                strcpy(requestUnsubscribe.base.userName, td.UserName);
                strcpy(requestUnsubscribe.base.FEED_PIPE, td.FEED_PIPE);
                write(td.manager_fd, &requestUnsubscribe, sizeof(RequestSubscribeUnsubscribeManager));

                return ;
            }else{
                printf(SYNTAX_ERROR_UNSUBSCRIBE);
                return ;
            }
        case 4: // CLEAR
            if(n_topics == 1){
                system("clear");
                printf("cmd > ");
                return ;
            }else{
                printf(SYNTAX_ERROR_CLEAR);
                return ;
            }
        default:
            return ;
    }
}

void feedView(const char *nome) {
    char buffer[MAX_MSG_SIZE];
    char FEED_PIPE[256];
    pthread_t monitor_thread, response_thread;
    RequestAuthManager requestAuthManager;
    TFEED td;

    //system("clear");
    sleep(1);

    if (access(MANAGER_PIPE, F_OK) != 0) {
        printf(MANAGER_NOT_RUNNING);
        exit(EXIT_FAILURE);
    }

    snprintf(FEED_PIPE, sizeof(FEED_PIPE), "../tmp/pipe_%d", getpid());
    strcpy(td.FEED_PIPE, FEED_PIPE);
    strcpy(td.UserName, nome);

    if(mkfifo(FEED_PIPE, 0660)==-1){
        perror(ERROR_CREATING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }




    td.manager_fd = open(MANAGER_PIPE, O_WRONLY);
    if (td.manager_fd  == -1) {
        perror(ERROR_OPENING_MANAGER_PIPE);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }


    if (pthread_create(&monitor_thread, NULL, monitorServer, (void *)FEED_PIPE) != 0) {
        perror(ERROR_CREATING_MONITOR_THREAD);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    requestAuthManager.type = LOGIN;
    strcpy(requestAuthManager.base.FEED_PIPE, td.FEED_PIPE);
    strcpy(requestAuthManager.base.userName, td.UserName);

    write(td.manager_fd, &requestAuthManager.type, sizeof(RequestType));
    write(td.manager_fd, &requestAuthManager, sizeof(RequestAuthManager));
    printf("%s\n", requestAuthManager.base.FEED_PIPE);

    int feed_fd = open(FEED_PIPE, O_RDONLY);
    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    ResponseType responseType;
    ResponseInfoError response_info_error;
    read(feed_fd, &responseType, sizeof(ResponseType));
    read(feed_fd, &response_info_error, sizeof(ResponseInfoError));

    if(responseType != LOGIN_SUCCESS){
        printf(response_info_error.buffer);
        close(feed_fd);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    printf(response_info_error.buffer);

    close(feed_fd);

    if (pthread_create(&response_thread, NULL, managerHandlerThread, (void *)&td) != 0) {
        perror(ERROR_CREATING_RESPONSE_THREAD);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    printf("cmd > ");
    fflush(stdout);

    do {


        if (fgets(buffer, MAX_MSG_SIZE, stdin) == NULL) {
            printf(ERROR_READING_COMMAND);
            continue;
        }

        buffer[strlen(buffer) - 1] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            requestAuthManager.type = LOGOUT;
            strcpy(requestAuthManager.base.FEED_PIPE, td.FEED_PIPE);
            strcpy(requestAuthManager.base.userName, td.UserName);
            write (td.manager_fd , &requestAuthManager.type, sizeof(RequestType));
            write(td.manager_fd , &requestAuthManager, sizeof(RequestAuthManager));
            system("clear");
            printf(EXITING);
            break;
        }

        processCommand(buffer,td);

    } while (1);


    close(td.manager_fd);
    unlink(FEED_PIPE);
}