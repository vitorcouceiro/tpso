#include "../controllers/manager_handler_thread.c"
#include "../controllers/monitor_server_thread.c"

int countWords(const char *buffer){
    int spaces = 0;

    for(int i = 0; buffer[i] != '\0'; i++){
        if(buffer[i] == ' '){
            spaces++;
        }
    }

    return spaces + 1;
}

int processCommand (const char *buffer){
    int index = -1;

    if(strcmp(buffer,"") == 0){
        return 0;
    }

    char buffer_copy[MAX_MSG_SIZE];
    strcpy(buffer_copy, buffer);


    char *command = strtok(buffer_copy, SPACE);

    int n_topics = countWords(buffer_copy);

    for(int i = 0; i < N_COMMANDS_USER; i++){
        if(strcmp(command, COMMANDS_USER[i]) == 0){
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(INVALID_COMMAND);
        return 0;
    }

    switch (index)
    {
        case 0: // TOPICS
            if(n_topics == 1){
                return 1;
            }else{
                printf(SYNTAX_ERROR_TOPICS);
                return 0;
            }
        case 1: // MSG
            if(n_topics >= 4){
                char *topic = strtok(NULL, SPACE);
                char *duration = strtok(NULL, SPACE);
                char *message = strtok(NULL, "");

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return 0;
                }

                if (strlen(message) > 300) {
                    printf(MESSAGE_LENGTH_ERROR);
                    return 0;
                }

                return 1;
            }else{
                printf(SYNTAX_ERROR_MSG);
                return 0;
            }
        case 2: // SUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return 0;
                }

                return 1;
            }else{
                printf(SYNTAX_ERROR_SUBSCRIBE);
                return 0;
            }
        case 3: // UNSUBSCRIBE
            if(n_topics == 2){
                char *topic = strtok(NULL, SPACE);

                if (strlen(topic) > 20) {
                    printf(TOPIC_LENGTH_ERROR);
                    return 0;
                }

                return 1;
            }else{
                printf(SYNTAX_ERROR_UNSUBSCRIBE);
                return 0;
            }
        default:
            return 0;
    }
}

void feedView(const char *nome) {
    char buffer[MAX_MSG_SIZE];
    char FEED_PIPE[256];
    pthread_t monitor_thread, response_thread;
    Comunicacao comunicacao;
    TFEED td;

    system("clear");
    sleep(1);

    if (access(MANAGER_PIPE, F_OK) != 0) {
        printf(MANAGER_NOT_RUNNING);
        exit(EXIT_FAILURE);
    }

    snprintf(FEED_PIPE, sizeof(FEED_PIPE), "../tmp/pipe_%d", getpid());
    strcpy(td.FEED_PIPE, FEED_PIPE);

    if(mkfifo(FEED_PIPE, 0660)==-1){
        perror(ERROR_CREATING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    strcpy(comunicacao.user.FEED_PIPE, FEED_PIPE);


    int manager_fd = open(MANAGER_PIPE, O_WRONLY);
    if (manager_fd == -1) {
        perror(ERROR_OPENING_MANAGER_PIPE);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }


    if (pthread_create(&monitor_thread, NULL, monitorServer, (void *)FEED_PIPE) != 0) {
        perror(ERROR_CREATING_MONITOR_THREAD);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    strcpy(comunicacao.tipoPedido, "linha_commands");

    strcpy(comunicacao.user.nome, nome);
    strcpy(comunicacao.tipoPedido, "login");

    write(manager_fd, &comunicacao, sizeof(Comunicacao));

    int feed_fd = open(FEED_PIPE, O_RDONLY);
    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    read(feed_fd, &comunicacao, sizeof(Comunicacao));

    if (strcmp(comunicacao.buffer, LOGIN_SUCCESS) != 0) {
        printf(comunicacao.buffer);
        close(feed_fd);
        unlink(FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    printf(comunicacao.buffer);


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

        if (strcmp(buffer, EXIT) == 0) {
            strcpy(comunicacao.tipoPedido,"logout");
            write(manager_fd, &comunicacao, sizeof(Comunicacao));
            system("clear");
            printf(EXITING);
            break;
        }

        if (processCommand(buffer) == 1) {
            strcpy(comunicacao.tipoPedido,"linha_commands");
            strcpy(comunicacao.buffer, buffer);
            write(manager_fd, &comunicacao, sizeof(Comunicacao));
        }

    } while (1);


    close(manager_fd);
    unlink(FEED_PIPE);
}