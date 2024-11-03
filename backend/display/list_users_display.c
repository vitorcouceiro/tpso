void displayListUsers(TDATA *td){
    if(td->n_users == 0){
        printf(NO_USERS_CONNECTED);
    }else{
        printf("Utilizadores conectados:\n");
        for(int i = 0; i < td->n_users; i++){
            printf("-> %s \n",td->user[i].nome);
        }
    }
}
