#ifndef USER_SERVICE_H
#define USER_SERVICE_H
#include "../manager.h"
void expelUser(TDATA *td, char *buffer);
void handleLoginRequest(int manager_fd, TDATA *td);
void handleLogoutRequest(int manager_fd, TDATA *td);
#endif //USER_SERVICE_H
