#include "core/server.h"
#include "ui/console.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, (size_t)0);

    int c;
    _Bool server = 0;
    _Bool client = 0;
    int errflg = 0;

    while ((c = getopt(argc, argv, ":hsc")) != -1) {
        switch (c) {
        case 's':
            server = 1;
            break;
        case 'c':
            client = 1;
            break;
        case 'h':
            errflg++;
            break;
        case ':':
            fprintf(stderr, "Option -%c requires an operand\n", optopt);
            errflg++;
            break;
        case '?':
            fprintf(stderr, "Unrecognized option: '-%c'\n", optopt);
            errflg++;
        }
    }

    if (client + server == 0) {
        server = 1;
        client = 1;
    }


    return start_console(server, client);

    if (errflg) {
        fprintf(stderr, "\n");
        fprintf(stderr, "Usage:\ttransmitr [OPTIONS]\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "Transmitr - Easily transfer stuff across your computer(s)\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -s\tRun server (Default: enabled - disables the client if not specified with '-c')\n");
        fprintf(stderr, "  -c\tRun client (Default: enabled - disables the server if not specified with '-s')\n");
        fprintf(stderr, "  -h\tShow help\n");
        return (2);
    }
}
