
#include "console.h"

#include "../core/client.h"
#include "../utils/strings.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

int start_console() {
    char *input = NULL;
    _Bool keep_running = 1;

    Client cl;
    client_init(&cl);
    int localhost_fd = client_connect(&cl, "127.0.0.1");

    printf("Please specify filename to send (write 'exit'/'q' to close the app)\n");
    while (keep_running) {
        input = readline("$ ");
        if (strcmp(input, "exit") == 0 || strcmp(input, "q") == 0) {
            keep_running = 0;
        } else {
            client_send_file(localhost_fd, trim(input));
        }
        free(input);
    }

    printf("Exiting the app.\n");
    client_destroy(&cl);
    return 0;
}
