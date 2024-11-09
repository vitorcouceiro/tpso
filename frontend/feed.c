#include "feed.h"
#include "../utils/includes.h"
#include "../frontend/view/feed_view.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(INVALID_ARGS_FEED);
        exit(EXIT_FAILURE);
    }
    feedView(argv[1]);
}