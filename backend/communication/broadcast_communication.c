void broadcastMsg(TDATA *td, Comunicacao comunicacao) {
    for (int i = 0; i < td->n_users; i++) {
        strcpy(comunicacao.user.FEED_PIPE, td->user[i].FEED_PIPE);
        unicastMsg(comunicacao);
    }
}
