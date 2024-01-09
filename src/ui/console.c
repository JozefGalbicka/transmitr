
#include "console.h"

#include "../core/client.h"
#include "../core/server.h"
#include "../utils/strings.h"
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

char *valid_modes[] = {"raw", "huff", "lzw"};
int modes_count = 3;
char mode = 'r';

static _Bool is_valid_mode(const char *mode) {

    for (int i = 0; i < modes_count; i++) {
        if (strcmp(mode, valid_modes[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static void print_valid_modes() {
    printf("Available modes:\n");
    for (int i = 0; i < modes_count; i++) {
        printf("  - '%s'\n", valid_modes[i]);
    }
}

int start_console(_Bool server, _Bool client) {
    char *input = NULL;

    _Bool keep_running = 1;

    Client cl;
    int last_fd;
    char mode = 'r';

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
        int input_len = strlen(input);

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

                } else if (input_len == 5 && strncmp(input, ":mode", 5) == 0) {
                    printf("Current mode is: '%c'\n", mode);
                    print_valid_modes();
                } else if (input_len > 6 && strncmp(input, ":mode ", 6) == 0) {
                    if (is_valid_mode(input + 6)) {
                        mode = *(input + 6);
                        printf("Mode set to '%c'\n", mode);
                    } else {
                        printf("Invalid mode.\n");
                    print_valid_modes();
                    }

                } else {
                    client_send_file_to_all(&cl, trim(input), mode);
                }
            }
        }
        free(input);
    }
    return 0;
}
