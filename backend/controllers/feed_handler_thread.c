#include "../communication/unicast_communication.c"
#include "../service/msg_service.c"
#include "../service/topic_service.c"

void *feedHandlerThread(void *ptdata) {
    int index = -1;
    TDATA *td = (TDATA *)ptdata;
    Comunicacao comunicacao;

    int manager_fd = open(MANAGER_PIPE, O_RDWR);
    if (manager_fd == -1) {
        perror(ERROR_OPENING_MANAGER_PIPE);
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        read(manager_fd, &comunicacao, sizeof(Comunicacao));
        
        if (strcmp(comunicacao.tipoPedido, "login") == 0) {
            if (td->n_users == MAX_USERS) {
                strcpy(comunicacao.buffer, MAX_USERS_REACHED);
            } else {
                int user_exists = 0;
                for (int i = 0; i < td->n_users; i++) {
                    if (strcmp(td->user[i].nome, comunicacao.user.nome) == 0) {
                        user_exists = 1;
                        break;
                    }
                }
                if (user_exists) {
                    strcpy(comunicacao.buffer, USERNAME_ALREADY_EXISTS);
                } else {
                    // Adicionar user
                    strcpy(comunicacao.buffer, LOGIN_SUCCESS);
                    strcpy(td->user[td->n_users].nome, comunicacao.user.nome);
                    strcpy(td->user[td->n_users].FEED_PIPE, comunicacao.user.FEED_PIPE);
                    td->n_users++;
                }
            }
            unicastMsg(comunicacao);
        } else if (strcmp(comunicacao.tipoPedido, "logout") == 0) {
            for (int i = 0; i < td->n_users; i++) {
                if (strcmp(comunicacao.user.nome, td->user[i].nome) == 0) {
                    for (int j = i; j < td->n_users - 1; j++) {
                        strcpy(td->user[j].nome, td->user[j + 1].nome);
                    }
                    td->n_users--;
                }
            }
        } else if (strcmp(comunicacao.tipoPedido, "linha_commands") == 0) {
            char *command = strtok(comunicacao.buffer, SPACE);
            strcpy(comunicacao.command, command);

            for (int i = 0; i < N_COMMANDS_USER; i++) {
                if (strcmp(command, COMMANDS_USER[i]) == 0) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                strcpy(comunicacao.buffer, INVALID_COMMAND);
                unicastMsg(comunicacao);
                continue;
            }

            switch (index) {
                case 0: // TOPICS
                    sendTopics(comunicacao, td);
                    break;
                case 1: // MSG
                    createMsg(comunicacao, td);
                    break;
                case 2: // SUBSCRIBE
                    // subscribeTopics();
                    break;
                case 3: // UNSUBSCRIBE
                    // unsubscribeTopics();
                    break;
                case 4: // HELP
                    // helpUser();
                    break;
                default:
                    strcpy(comunicacao.buffer, INVALID_COMMAND);
                    unicastMsg(comunicacao);
                    continue;
            }

        }
    }
    close(manager_fd);
}
