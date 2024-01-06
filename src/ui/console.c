
#include "console.h"

#include "../core/client.h"
#include "../core/server.h"
#include "../utils/strings.h"
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

int start_console(_Bool server, _Bool client) {
    char *input = NULL;

    _Bool keep_running = 1;

    Client cl;
    int last_fd;

    pthread_t thread_id;

    if (server) {
        pthread_create(&thread_id, NULL, run_server, &keep_running);
    }
    if (client) {
        client_init(&cl);
    }

    printf("Please specify filename to send (write 'exit'/'q' to close the app)\n");
    while (keep_running) {
        input = readline("$ ");

        if (strcmp(input, "exit") == 0 || strcmp(input, "q") == 0) {
            keep_running = 0;
            printf("Exiting the app.\n");
            if (client) {
                client_destroy(&cl);
                printf("Client connection closed.\n");
            }
            if (server) {
                pthread_join(thread_id, NULL);
                printf("Server closed.\n");
            }
        } else {

            if (client) {
                if (strncmp(input, ":connect ", 9) == 0) {
                    printf("Connecting to '%s'\n", input + 9);
                    last_fd = client_connect(&cl, input + 9);
                } else if (strcmp(input, "") == 0) {
                    ;
                } else {
                    client_send_file_to_all(&cl, trim(input));
                }
            }
        }
        free(input);
    }
    return 0;
}
