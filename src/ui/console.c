
#include "console.h"

#include "../core/client.h"
#include "../utils/strings.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <string.h>

int start_console() {
    printf("Please specify filename to send\n");
    char file_name[50];
    while (1) {
        strcpy(file_name, readline("$ "));
        client_send_file(trim(file_name));
    }
    return 0;
}
