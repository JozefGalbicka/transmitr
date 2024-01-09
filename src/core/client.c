
#include "client.h"
#include "../utils//macros.h"
#include "../utils/strings.h"
#include "compression_algorithms/LZW/LZW_core.h"
#include "compression_algorithms/huffman/code_table.h"
#include "compression_algorithms/huffman/huffman_core.h"
#include "header.h"
#include "structures/tree/red_black_tree.h"

#include <arpa/inet.h>
#include <linux/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define nofile "File Not Found!"
#define PORT 8080

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

    memcpy(header.flags, &mode, 1);
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
        } else if (mode == 'h') {
            CodeTable code_table;
            code_table_init(&code_table);

            unsigned char *compressed = malloc(read_size);
            int compressed_size = 0;
            int compressed_last_bits_valid = 0;

            huffman_encode(buf, read_size, compressed, &compressed_size, &compressed_last_bits_valid, &code_table);

            size_t table_buf_size = 0;
            unsigned char *table_buf = code_table_serialize(&code_table, &table_buf_size, compressed_last_bits_valid);

            //// SENDING TABLE
            // Sending header
            strcpy(header.flags, "hc");
            header.data_length = table_buf_size;
            sent_len = send(client_fd, serialize_header(&header, header_buf), 16, 0);
            DEBUG_MESSAGE("%zdu (Header)\n", sent_len);

            // Sending data
            sent_len = send(client_fd, table_buf, table_buf_size, 0);
            if (sent_len != table_buf_size) {
                printf("Sent/Buffer: %zd / %zu\n", sent_len, table_buf_size);
            }
            byte_counter += sent_len;
            DEBUG_MESSAGE("%zdu (Data (CodeTable))\n", sent_len);
            code_table_destroy(&code_table);
            free(table_buf);

            //// SENDING COMPRESSED DATA
            // Sending header
            strcpy(header.flags, "hd");
            header.data_length = compressed_size;
            sent_len = send(client_fd, serialize_header(&header, header_buf), 16, 0);
            DEBUG_MESSAGE("%zdu (Header)\n", sent_len);

            // Sending data
            sent_len = send(client_fd, compressed, compressed_size, 0);
            if (sent_len != compressed_size) {
                printf("Sent/Buffer: %zd / %d\n", sent_len, compressed_size);
            }
            free(compressed);
            byte_counter += sent_len;
            DEBUG_MESSAGE("%zdu (Data (Compressed))\n", sent_len);
        } else if (mode == 'l') {
            // Init
            RBTree tree;
            red_black_tree_init(&tree);

            // Compressing data
            size_t compressed_count = 0;
            int *compressed = lzw_compress_encode(buf, &tree, &compressed_count, read_size);

            // Serializing compressed data
            unsigned char *compressed_serialized = malloc(compressed_count * sizeof(uint32_t));
            for (int i = 0; i < compressed_count; i++) {
                uint32_t data = htonl(compressed[i]);
                memcpy(compressed_serialized + i * sizeof(data), &data, sizeof(data));
            }
            DEBUG_MESSAGE("LZW Compressed/Raw %zu/%d B\n", compressed_count * sizeof(uint32_t), read_size);
#include "../utils/prints.h"
            printf("Komprimovane data: ");
            for (int i = 0; i < compressed_count; i++) {
                printf("%d ", compressed[i]);
            }
            printf("\n");
            print_bytes(compressed, 4);
            print_bytes(compressed_serialized, 4);
            free(compressed);

            // Serializing Tree
            size_t tree_buf_size = 0;
            unsigned char *tree_buf = red_black_tree_serialize(&tree, &tree_buf_size);
            DEBUG_MESSAGE("Tree buf size: %zu B\n", tree_buf_size);
            red_black_tree_destroy(&tree);

            //// SENDING TREE
            // Sending header
            strcpy(header.flags, "lt");
            header.data_length = tree_buf_size;
            sent_len = send(client_fd, serialize_header(&header, header_buf), 16, 0);
            DEBUG_MESSAGE("%zdu (Header)\n", sent_len);

            // Sending data
            sent_len = send(client_fd, tree_buf, tree_buf_size, 0);
            if (sent_len != tree_buf_size) {
                printf("Sent/Buffer: %zd / %zu\n", sent_len, tree_buf_size);
            }
            byte_counter += sent_len;
            DEBUG_MESSAGE("%zdu (Data (Tree))\n", sent_len);

            //// SENDING COMPRESSED DATA
            // Sending header
            strcpy(header.flags, "ld");
            header.data_length = compressed_count * sizeof(uint32_t);
            sent_len = send(client_fd, serialize_header(&header, header_buf), 16, 0);
            DEBUG_MESSAGE("%zdu (Header)\n", sent_len);

            // Sending data
            sent_len = send(client_fd, compressed_serialized, compressed_count * sizeof(uint32_t), 0);
            if (sent_len != compressed_count * sizeof(uint32_t)) {
                printf("Sent/Buffer: %zd / %lu\n", sent_len, compressed_count * sizeof(uint32_t));
            }
            byte_counter += sent_len;
            DEBUG_MESSAGE("%zdu (Data (Compressed))\n", sent_len);

            free(compressed_serialized);
            free(tree_buf);
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
