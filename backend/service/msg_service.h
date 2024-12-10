#ifndef MSG_SERVICE_H
#define MSG_SERVICE_H
#include "../manager.h"

void readtxt(char *filename, TDATA *td) ;
void createMsg(int manager_fd, TDATA *td);
void saveMessages(char filename[], TDATA *td);

#endif //MSG_SERVICE_H
