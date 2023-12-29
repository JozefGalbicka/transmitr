
#include "console.h"

#include "../core/client.h"
#include "../utils/strings.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <string.h>

int start_console() {
    printf("Please specify filename to send\n");
    char file_name[50];

    Client cl;
    client_init(&cl);
    int localhost_fd = client_connect(&cl, "127.0.0.1");
    while (1) {
        strcpy(file_name, readline("$ "));
        client_send_file(localhost_fd, trim(file_name));
    }

    client_destroy(&cl);
    return 0;
}
