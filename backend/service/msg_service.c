void createMsg(Comunicacao comunicacao,TDATA *td){
    int index = -1;

    char *command = strtok(comunicacao.buffer, SPACE);
    char *topic = strtok(NULL, SPACE);
    char *duration = strtok(NULL, SPACE);
    char *message = strtok(NULL, "");

    for(int i = 0; i < td->n_topics;i++){
        if(strcmp(topic, td->topic[i].nome) == 0){
            index = i;
            break;
        }
    }

    // se o topico nao existir
    if(index == -1){
        if(td->n_topics >= MAX_TOPICS){
            strcpy(comunicacao.buffer,"Maximo de topicos atingido\n");
            unicastMsg(comunicacao);
            return;
        }else{
            strcpy(td->topic[td->n_topics].nome,topic);
            td->topic[td->n_topics].n_persistentes = 0;
            strcpy(td->topic[td->n_topics].estado,"desbloqueado");
            td->n_topics ++;
        }
    }else{
        if(duration == 0){// mensagem nao persistente

        }else{ //mensagem persistente

        }

    }
    unicastMsg(comunicacao);
}
