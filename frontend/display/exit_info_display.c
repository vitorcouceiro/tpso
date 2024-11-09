#include "exit_info_display.h"
#include "../../utils/includes.h"
#include "../../backend/models/comunicacao.h"

void displayExitInfo(Comunicacao comunicacao) {
    printf(USER_REMOVED_SPECIFIC,comunicacao.buffer);
}
