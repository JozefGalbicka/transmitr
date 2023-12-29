
#include "client.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define nofile "File Not Found!"

void client_init(Client *self) {
    self->servers = malloc(sizeof(ArrayList));
    array_list_init(self->servers, sizeof(int));
}

void client_destroy(Client *self) {
    client_close_all_connections(self);

    array_list_destroy(self->servers);
    free(self->servers);
    self->servers = NULL;
}

// function reading file 'fp' into chunk 'buf' of size 's'
static int read_chunk(FILE *fp, char *buf, int s) {
    buf[0] = '\0';

    int i, len;
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        fprintf(stderr, "ERROR READING FILE (== NULL)");
        return 1;
    }

    char ch;
    for (i = 0; i < s - 1; i++) {
        ch = fgetc(fp);
        buf[i] = ch;
        if (ch == EOF) {
            buf[i] = '\0';
            return 1;
        }
    }
    buf[s - 1] = '\0';
    return 0;
}

int client_connect(Client *self, const char *ip_addr) {
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    // char buffer[1024] = {0};
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    array_list_add(self->servers, &client_fd);
    return client_fd;
}

void client_close_all_connections(Client *self) {
    ArrayListIterator it;
    array_list_iterator_init(&it, self->servers);
    while (array_list_iterator_has_next(&it)) {
        void *tmp = array_list_iterator_move_next(&it);
        close(*(int *)tmp);
    }
    array_list_iterator_destroy(&it);
}

int client_send_file(int client_fd, const char *file_name) {
    // SENDING THE DATA
    FILE *fp = fopen(file_name, "r");

    printf("\nFile Name Received: %s\n", file_name);
    if (fp == NULL)
        printf("\nFile open failed!\n");
    else
        printf("\nFile Successfully opened!\n");

    char *buf;
    size_t buf_size = 4096;
    buf = malloc(buf_size);
    ssize_t sent_len = 0;

    printf("Sending file '%s'\n", file_name);
    while (read_chunk(fp, buf, buf_size) == 0) {
        sent_len = send(client_fd, buf, strlen(buf), 0);
        printf("%zdu\n", sent_len);
        // printf("%s", buf);
        // printf("%lu\n", strlen(buf));
    }
    sent_len = send(client_fd, buf, strlen(buf), 0);
    printf("%zdu\n", sent_len);
    printf("\nFinished sending the file '%s'\n", file_name);

    fclose(fp);
    free(buf);

    // RECEIVE THE DATA
    // valread = read(client_fd, buffer,
    //               1024 - 1); // subtract 1 for the null terminator at the end
    // printf("%s\n", buffer);

    return 0;
}

