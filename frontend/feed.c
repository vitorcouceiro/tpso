#include "../utils/includes.h"
#include "feed.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(INVALID_ARGS_FEED);
        exit(EXIT_FAILURE);
    }
    feedView(argv[1]);
}