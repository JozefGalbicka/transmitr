
#include "console.h"

#include "../core/client.h"
#include "../utils/strings.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <string.h>

int start_console() {
    char file_name[50];
    _Bool keep_running = 1;

    Client cl;
    client_init(&cl);
    int localhost_fd = client_connect(&cl, "127.0.0.1");

    printf("Please specify filename to send (write 'exit'/'q' to close the app)\n");
    while (keep_running) {
        strcpy(file_name, readline("$ "));
        if (strcmp(file_name, "exit") == 0 || strcmp(file_name, "q") == 0) {
            keep_running = 0;
        } else {
            client_send_file(localhost_fd, trim(file_name));
        }
    }

    printf("Exiting the app.\n");
    client_destroy(&cl);
    return 0;
}
