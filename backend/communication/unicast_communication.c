void unicastMsg(Comunicacao comunicacao){
    int feed_fd = open(comunicacao.user.FEED_PIPE, O_WRONLY);

    if (feed_fd == -1) {
        perror(ERROR_OPENING_FEED_PIPE);
        exit(EXIT_FAILURE);
    }

    write(feed_fd, &comunicacao, sizeof(Comunicacao));
    close(feed_fd);
}
