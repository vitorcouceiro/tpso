#ifndef FEED_VIEW_H
#define FEED_VIEW_H
#include "../feed.h"

int countWords(char *buffer);
void processCommand (const char *buffer, TFEED td);
void feedView(const char *nome);

#endif //FEED_VIEW_H
