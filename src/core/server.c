
// Server side C/C++ program to demonstrate Socket
// programming
#include "server.h"
#include "../structures/list/array_list.h"
#include "../utils/macros.h"
#include "compression_algorithms/LZW/LZW_core.h"
#include "compression_algorithms/huffman/code_table.h"
#include "compression_algorithms/huffman/huffman_core.h"
#include "header.h"
#include "structures/tree/red_black_tree.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 8080
#include <sys/socket.h>
#include <unistd.h>

static void *serve_client(void *cti_v) {
#define BUF_SIZE (4 * 1024 * 1024)
    ClientThreadInfo *cti = (ClientThreadInfo *)(cti_v);
    unsigned char *buffer = malloc(BUF_SIZE + 1);
    memset(buffer, 0, BUF_SIZE + 1);

    Header h;        // Used to store the header
    _Bool first = 1; // Denotes whether it's time to read new header

    // Special use-case when header is separated into two different reads
    _Bool is_split_header = 0;        // Denotes if header part was segmented into two different reads()
    unsigned char tmp_head_buf[16];   // buffer for segmented headers
    uint32_t tmp_head_buf_loaded = 0; // how many bytes of header were loaded into buffer

    ssize_t valread;                    // Number of bytes read() from the socket
    uint32_t buffer_index = 0;          // Current index (position) in the buffer
    unsigned char *cur = buffer;        // Current pointer (position) in the buffer
    uint32_t chunk_remaining_bytes = 0; // Remaining bytes in the chunk/message
    unsigned char *start = NULL;        // Pointer to start of the found data
    int start_size = 0;                 // Size of the found data

    char file_name[100];
    FILE *f;

    // Huffman CodeTable
    CodeTable huff_code_table;
    unsigned char *huff_code_table_buf = NULL;
    size_t huff_code_table_buf_size = 0;
    int huff_compressed_last_bits_valid = -1;
    // Huffman-compressed data
    unsigned char *huff_compressed_data = NULL;
    size_t huff_compressed_data_size = 0;

    // LZW Tree
    RBTree lzw_tree;
    unsigned char *lzw_tree_buf = NULL;
    size_t lzw_tree_buf_size = 0;
    // LZW-Compressed data
    unsigned char *lzw_compressed_data = NULL;
    int *lzw_compressed_data_deserialized = NULL;
    size_t lzw_compressed_data_size = 0;

    int byte_counter = 0;
    int byte_counter_from_read = 0;
    int byte_counter_from_header = 0;

    while ((valread = read(cti->client_fd, buffer, BUF_SIZE)) != 0) {
        // https://stackoverflow.com/questions/3074824/reading-buffer-from-socket
        // buffer[valread] = 0;

        // Initialize current pointer on the buffer
        cur = buffer;
        buffer_index = 0;

        DEBUG_MESSAGE("\n--- %zd B Received\n", valread);
        while (buffer_index < valread) { // while != last index
            // DEBUG_MESSAGE("--New-cycle\n");

            if (first) { // time to read new header

                if (!is_split_header) { // reading new header

                    if (buffer_index + 16 <= valread) { // get entire header
                        deserialize_header(&h, cur);
                        header_print(&h);
                        first = 0;
                        cur += 16;
                        buffer_index += 16;
                    } else { // get part of the header, rest will be in another read
                        is_split_header = 1;

                        memcpy(&tmp_head_buf, cur, valread - buffer_index);
                        tmp_head_buf_loaded = (valread - buffer_index);

                        cur += (valread - buffer_index);
                        buffer_index += (valread - buffer_index);

                        // DEBUG_MESSAGE("tmp_head_buf_loaded: %u\n",
                        //         tmp_head_buf_loaded);

                        continue; // could/should be break, but for debug
                                  // purposes (that all variables have right
                                  // values) we are using continue for now
                    }

                } else { // reading rest of the header
                    // DEBUG_MESSAGE("Entering split-header case\n");

                    uint32_t remaining_buffer_size = 16 - tmp_head_buf_loaded;

                    memcpy(&tmp_head_buf[tmp_head_buf_loaded], cur, remaining_buffer_size);
                    cur += remaining_buffer_size;
                    buffer_index += remaining_buffer_size;

                    deserialize_header(&h, tmp_head_buf);
                    header_print(&h);

                    tmp_head_buf_loaded = 0;
                    is_split_header = 0;
                    first = 0;
                }

                byte_counter_from_header += h.data_length;
                chunk_remaining_bytes = h.data_length;
            }

            if (buffer_index >= valread) {
                if (chunk_remaining_bytes == 0) { // if message was empty
                    first = 1;
                }
                break;
            }

            if (chunk_remaining_bytes >= (valread - buffer_index)) { // rest of the buffer is actual data
                start = cur;
                start_size = valread - buffer_index;

                chunk_remaining_bytes -= (valread - buffer_index);
                buffer_index += (valread - buffer_index);

                DEBUG_MESSAGE("\nENTIRE BUFFER USED\n");
                DEBUG_MESSAGE("chunk_remaining: %d\n", chunk_remaining_bytes);
                DEBUG_MESSAGE("buffer_index: %d\n", buffer_index);
                DEBUG_MESSAGE("valread: %zd\n\n", valread);
            } else { // there is another header in the buffer somewhere
                start = cur;
                start_size = chunk_remaining_bytes;

                cur += chunk_remaining_bytes;
                buffer_index += chunk_remaining_bytes; // == valread
                chunk_remaining_bytes = 0;

                DEBUG_MESSAGE("\nPARTIAL BUFFER USED\n");
                DEBUG_MESSAGE("chunk_remaining: %d\n", chunk_remaining_bytes);
                DEBUG_MESSAGE("buffer_index: %d\n", buffer_index);
                DEBUG_MESSAGE("valread: %d\n\n", (int)valread);
            }

            if (*h.flags == 's') {
                DEBUG_MESSAGE("--START FILENAME: ");
                fprintf(stdout, "Receiving file '%.*s'\n", start_size, start);
                memcpy(file_name, start, start_size <= 100 ? start_size : 100);
                f = fopen(file_name, "wb");
                if (!f) {
                    fprintf(stderr, "Error occurred with opening file '%s' for write", file_name);
                }
                setvbuf(f, NULL, _IONBF, 0);
                byte_counter_from_header = 0;
                byte_counter_from_read = 0;
                byte_counter = 0;
            } else if (*h.flags == 'e') {
                DEBUG_MESSAGE("--END FILENAME: ");
                fprintf(stdout, "Successfully received file '%.*s'\n", start_size, start);
                fclose(f);
                f = NULL;
                DEBUG_MESSAGE("bytes from header: %d\n", byte_counter_from_header);
                DEBUG_MESSAGE("bytes from reads: %d\n", byte_counter_from_read);
                printf("bytes from processing: %d\n", byte_counter);
            } else {
                // fprintf(f, "%.*s", (int)valread - buffer_index, cur);
                if (*h.flags == 'r') { // RAW
                    fwrite(start, 1, start_size, f);
                } else if (*h.flags == 'h') { // HUFFMAN
                    byte_counter_from_read += start_size;

                    if (*(h.flags + 1) == 'c') { // CODETABLE
                        // printf("Received Huffman CodeTable\n");

                        if (chunk_remaining_bytes == 0) {

                            if (!huff_code_table_buf) {
                                code_table_deserialize(&huff_code_table, start, 0, &huff_compressed_last_bits_valid);
                                byte_counter += start_size;
                            } else {
                                memcpy(huff_code_table_buf + huff_code_table_buf_size, start, start_size);
                                huff_code_table_buf_size += start_size;
                                byte_counter += start_size;

                                code_table_deserialize(&huff_code_table, huff_code_table_buf, 0,
                                                       &huff_compressed_last_bits_valid);
                                free(huff_code_table_buf);
                                huff_code_table_buf = NULL;
                                huff_code_table_buf_size = 0;
                            }
                        } else { // CodeTable received in two separate reads
                            if (!huff_code_table_buf) {
                                huff_code_table_buf = malloc(h.data_length);
                            }
                            memcpy(huff_code_table_buf + huff_code_table_buf_size, start, start_size);
                            huff_code_table_buf_size += start_size;
                            byte_counter += start_size;
                        }

                    } else if (*(h.flags + 1) == 'd') { // COMPRESSED DATA
                        // printf("Received Huffman-compressed data");
                        if (huff_compressed_last_bits_valid == -1) {
                            fprintf(stderr, "Reading data, but CodeTable was not initialized");
                            exit(98);
                        }

                        if (chunk_remaining_bytes == 0) {
                            unsigned char *decrypted = malloc(2 * h.data_length);
                            int decrypted_size = 0;

                            DEBUG_MESSAGE("Decoding..\n");
                            if (!huff_compressed_data) {
                                byte_counter += start_size;
                                huffman_decode(start, start_size, huff_compressed_last_bits_valid, &huff_code_table,
                                               decrypted, &decrypted_size);
                            } else {
                                byte_counter += start_size;
                                memcpy(huff_compressed_data + huff_compressed_data_size, start, start_size);
                                huff_compressed_data_size += start_size;

                                huffman_decode(huff_compressed_data, huff_compressed_data_size,
                                               huff_compressed_last_bits_valid, &huff_code_table, decrypted,
                                               &decrypted_size);
                                free(huff_compressed_data);
                                huff_compressed_data = NULL;
                                huff_compressed_data_size = 0;
                            }
                            DEBUG_MESSAGE("Finished decoding\n");

                            fwrite(decrypted, 1, decrypted_size, f);
                            code_table_destroy(&huff_code_table);
                            free(decrypted);

                        } else { // Huffman-compressed data received in two separate reads
                            if (!huff_compressed_data) {
                                huff_compressed_data = malloc(h.data_length);
                            }
                            byte_counter += start_size;
                            memcpy(huff_compressed_data + huff_compressed_data_size, start, start_size);
                            huff_compressed_data_size += start_size;
                        }
                    } else {
                        fprintf(stderr, "Missing second flag in Huffman header\n");
                        exit(97);
                    }
                } else if (*h.flags == 'l') { // LZW
                    byte_counter_from_read += start_size;

                    if (*(h.flags + 1) == 't') { // TREE
                        // printf("Received LZW Tree\n");

                        if (chunk_remaining_bytes == 0) {

                            if (!lzw_tree_buf) {
                                byte_counter += start_size;

                                red_black_tree_init(&lzw_tree);
                                red_black_tree_deserialize(&lzw_tree, start, h.data_length);
                            } else {
                                memcpy(lzw_tree_buf + lzw_tree_buf_size, start, start_size);
                                lzw_tree_buf_size += start_size;
                                byte_counter += start_size;

                                red_black_tree_init(&lzw_tree);
                                red_black_tree_deserialize(&lzw_tree, lzw_tree_buf, h.data_length);

                                free(lzw_tree_buf);
                                lzw_tree_buf = NULL;
                                lzw_tree_buf_size = 0;
                            }
                        } else { // Tree received in two/more separate reads
                            if (!lzw_tree_buf) {
                                lzw_tree_buf = malloc(h.data_length);
                            }
                            memcpy(lzw_tree_buf + lzw_tree_buf_size, start, start_size);
                            lzw_tree_buf_size += start_size;
                            byte_counter += start_size;
                        }

                    } else if (*(h.flags + 1) == 'd') { // COMPRESSED DATA
                        // printf("Received LZW-compressed data");

                        if (chunk_remaining_bytes == 0) {
                            unsigned char *decompressed = NULL;
                            size_t decompressed_size = 0;

                            if (!lzw_compressed_data) {
                                DEBUG_MESSAGE("Decoding from single read()..\n");
                                byte_counter += start_size;

                                lzw_compressed_data_deserialized = malloc(h.data_length);
                                // printf("Compressed data:\n");
                                for (int i = 0; i < start_size; i = i + sizeof(uint32_t)) {
                                    uint32_t data = 0;
                                    memcpy(&data, start + i, sizeof(data));
                                    int deserialized_data = ntohl(data);
                                    // printf("%d ", deserialized_data);
                                    memcpy((unsigned char *)lzw_compressed_data_deserialized + i, &deserialized_data,
                                           sizeof(data));
                                }

                                decompressed =
                                    lzw_decompress_encode(lzw_compressed_data_deserialized,
                                                          start_size / sizeof(uint32_t), &decompressed_size, &lzw_tree);
                                free(lzw_compressed_data_deserialized);
                                lzw_compressed_data_deserialized = NULL;

                            } else {
                                DEBUG_MESSAGE("Decoding from multiple read()'s..\n");
                                byte_counter += start_size;
                                memcpy(lzw_compressed_data + lzw_compressed_data_size, start, start_size);
                                lzw_compressed_data_size += start_size;

                                lzw_compressed_data_deserialized = malloc(h.data_length);

                                for (int i = 0; i < lzw_compressed_data_size; i = i + sizeof(uint32_t)) {
                                    uint32_t data = 0;
                                    memcpy(&data, lzw_compressed_data + i, sizeof(data));
                                    int deserialized_data = ntohl(data);
                                    memcpy((unsigned char *)lzw_compressed_data_deserialized + i, &deserialized_data,
                                           sizeof(data));
                                }
                                free(lzw_compressed_data);

                                decompressed = lzw_decompress_encode(lzw_compressed_data_deserialized,
                                                                     lzw_compressed_data_size / sizeof(uint32_t),
                                                                     &decompressed_size, &lzw_tree);
                                free(lzw_compressed_data_deserialized);
                                lzw_compressed_data_deserialized = NULL;

                                lzw_compressed_data = NULL;
                                lzw_compressed_data_size = 0;
                            }
                            DEBUG_MESSAGE("Finished decoding, writing %zu B\n", decompressed_size);

                            fwrite(decompressed, 1, decompressed_size, f);
                            red_black_tree_destroy(&lzw_tree);
                            free(decompressed);

                        } else { // LZW-compressed data received in two/more separate reads
                            if (!lzw_compressed_data) {
                                lzw_compressed_data = malloc(h.data_length);
                            }
                            byte_counter += start_size;
                            memcpy(lzw_compressed_data + lzw_compressed_data_size, start, start_size);
                            lzw_compressed_data_size += start_size;
                        }

                    } else {
                        fprintf(stderr, "Missing second flag in Huffman header\n");
                        exit(97);
                    }

                } else {
                    fprintf(stderr, "Missing flag in FILE transfer\n");
                    exit(96);
                }
            }

            if (chunk_remaining_bytes == 0) {
                first = 1;
            }
        }
    }
    free(buffer);
    cti->running = 0;
    return NULL;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void *run_server(void *keep_running_v) {
    struct pollfd pfds[1];
    int fd_count = 0;

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pfds[0].fd = server_fd;
    pfds[0].events = POLLIN;
    fd_count = 1;

    pthread_t thread_id;
    struct sockaddr_storage remoteaddr;
    char remoteIP[100];

    ArrayList client_thread_infos;
    ClientThreadInfo *cti = NULL;

    array_list_init(&client_thread_infos, sizeof(ClientThreadInfo **));

    while (*(_Bool *)keep_running_v) {
        int poll_count = poll(pfds, fd_count, 1000);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }
        if (pfds[0].revents & POLLIN) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&remoteaddr, &addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            cti = malloc(sizeof(ClientThreadInfo));
            cti->running = 1;
            cti->client_fd = new_socket;
            pthread_create(&thread_id, NULL, serve_client, cti);
            cti->thread_id = thread_id;

            array_list_add(&client_thread_infos, &cti);
            cti = NULL;

            fprintf(stdout, "New client connected: %s\n",
                    inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP,
                              INET6_ADDRSTRLEN));
        }
    }

    // Waiting for every client thread to exit
    ArrayListIterator it;
    void *tmp;
    array_list_iterator_init(&it, &client_thread_infos);
    while (array_list_iterator_has_next(&it)) {
        fprintf(stderr, "Closing client thread..\n");
        tmp = array_list_iterator_move_next(&it);
        cti = *(ClientThreadInfo **)tmp;
        pthread_join(cti->thread_id, NULL);
        free(cti);
    }
    array_list_iterator_destroy(&it);

    array_list_destroy(&client_thread_infos);

    //  closing the listening socket
    close(server_fd);
    return 0;
}
