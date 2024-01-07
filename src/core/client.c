
#include "client.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define nofile "File Not Found!"
#include "../utils//macros.h"
#include "../utils/strings.h"
#include "header.h"

#include <linux/tcp.h>

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
static int read_chunk(FILE *fp, unsigned char *buf, int s, int *read_size) {
    buf[0] = '\0';

    int i, len;
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        fprintf(stderr, "ERROR READING FILE (== NULL)");
        return 1;
    }

    unsigned char ch;
    int flag;
    for (i = 0; i < s - 1; i++) {
        flag = fgetc(fp);
        ch = flag;
        buf[i] = ch;
        if (flag == EOF) {
            buf[i] = '\0';
            *read_size = i;
            return 1;
        }
    }
    buf[s - 1] = '\0';
    *read_size = i;
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

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
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

void client_send_file_to_all(Client *self, const char *path, const char mode) {
    ArrayListIterator it;
    array_list_iterator_init(&it, self->servers);
    while (array_list_iterator_has_next(&it)) {
        void *tmp = array_list_iterator_move_next(&it);

        struct sockaddr_in addr;
        socklen_t addr_size = sizeof(struct sockaddr_in);
        int res = getpeername(*(int *)tmp, (struct sockaddr *)&addr, &addr_size);
        char clientip[20];
        strcpy(clientip, inet_ntoa(addr.sin_addr));
        printf("Sending to server '%s'", clientip);

        client_send_file(*(int *)tmp, path, mode);
    }
    array_list_iterator_destroy(&it);
}

int client_send_file(int client_fd, const char *path, const char mode) {
    const char *file_name = get_basename(path);
    int byte_counter = 0;
    // SENDING THE DATA
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        printf("\n'%s' - Open failed! Skipping!\n", file_name);
        return -1;
    } else {
        printf("\n'%s' - Successfully opened!\n", file_name);
    }

#define BUF_SIZE (4 * 1024 * 1024)

    unsigned char *buf = malloc(BUF_SIZE);
    ssize_t sent_len = 0;

    Header header = {.type = {0}, .flags = {0}, .data_length = 0};
    strcpy(header.type, "FILE");
    strcpy(header.flags, "s");
    header.data_length = strlen(file_name) + 1;

    unsigned char header_buf[16] = {0};
    printf("Sending file '%.*s'\n", 16, file_name);
    serialize_header(&header, header_buf);
    sent_len = send(client_fd, header_buf, 16, 0);
    DEBUG_MESSAGE("%zdu (Header)\n", sent_len);
    sent_len = send(client_fd, file_name, strlen(file_name) + 1, 0);
    DEBUG_MESSAGE("%zdu (Filename)\n", sent_len);


    strcpy(header.flags, &mode);
    int read_size = 0;
    while (read_chunk(fp, buf, BUF_SIZE, &read_size) == 0 || read_size != 0) {
        if (mode == 'r') {
            // Sending header
            header.data_length = read_size;
            sent_len = send(client_fd, serialize_header(&header, header_buf), 16, 0);
            DEBUG_MESSAGE("%zdu (Header)\n", sent_len);

            // Sending data
            sent_len = send(client_fd, buf, read_size, 0);
            if (sent_len != read_size) {
                printf("Sent/Buffer: %zd / %d\n", sent_len, read_size);
            }
            byte_counter += sent_len;
            DEBUG_MESSAGE("%zdu (Data)\n", sent_len);
        }
    }

    int flag = 1;
    setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

    strcpy(header.flags, "e");
    header.data_length = strlen(file_name) + 1;
    sent_len = send(client_fd, serialize_header(&header, header_buf), 16, 0);
    DEBUG_MESSAGE("%zdu (Header)\n", sent_len);

    sent_len = send(client_fd, file_name, strlen(file_name) + 1, 0);
    DEBUG_MESSAGE("%zdu (Filename)\n", sent_len);

    flag = 0;
    setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));

    printf("\nFinished sending the file '%s'\n", file_name);
    printf("Sent size: %d\n", byte_counter);

    fclose(fp);
    free(buf);
    free((void *)file_name);

    // ioctl(client_fd, SIOCOUTQ, &bytes);

    // RECEIVE THE DATA
    // valread = read(client_fd, buffer,
    //               1024 - 1); // subtract 1 for the null terminator at the end
    // printf("%s\n", buffer);

    return 0;
}
