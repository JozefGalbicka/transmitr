
// Server side C/C++ program to demonstrate Socket
// programming
#include "server.h"
#include "../utils/macros.h"
#include "header.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#include <pthread.h>

static void *serve_client(void *new_socket) {
#define BUF_SIZE (4 * 1024 * 1024)
    int client_fd = *(int *)(new_socket);
    unsigned char buffer[BUF_SIZE] = {0};

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

    while ((valread = read(client_fd, buffer, BUF_SIZE)) != 0) {
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
            } else if (*h.flags == 'e') {
                DEBUG_MESSAGE("--END FILENAME: ");
                fprintf(stdout, "Successfully received file '%.*s'\n", start_size, start);
                fclose(f);
                f = NULL;
            } else {
                // fprintf(f, "%.*s", (int)valread - buffer_index, cur);
                fwrite(start, 1, start_size, f);
            }

            if (chunk_remaining_bytes == 0) {
                first = 1;
            }
        }
    }
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int run_server() {
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

    while (1) {
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
            pthread_create(&thread_id, NULL, serve_client, &new_socket);
            fprintf(stdout, "New client connected: %s\n",
                    inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP,
                              INET6_ADDRSTRLEN));
        }
    }
    pthread_join(thread_id, NULL);

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    return 0;
}
