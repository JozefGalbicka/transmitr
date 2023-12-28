#include "core/server.h"
#include "ui/console.h"
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <server or client>\n", argv[0]);
        return 1;
    }
    if (argc != 2) {
        printf("Usage: %s <server or client>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        return run_server();
    } else if (strcmp(argv[1], "client") == 0) {
        return start_console();
    } else {
        printf("Invalid argument. Use 'server' or 'client'.\n");
        return 1;
    }

    return 0;
}
