
// Server side C/C++ program to demonstrate Socket
// programming
#include "server.h"
#include "../utils/macros.h"
#include "header.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int run_server(FILE *f) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
#define BUF_SIZE 4096
    unsigned char buffer[BUF_SIZE] = {0};
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
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

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

        while ((valread = read(new_socket, buffer, BUF_SIZE)) != 0) {
            // https://stackoverflow.com/questions/3074824/reading-buffer-from-socket
            // buffer[valread] = 0;

            // Initialize current pointer on the buffer
            cur = buffer;
            buffer_index = 0;

            DEBUG_MESSAGE("\n--%zd B\n", valread);
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

                // rest of the buffer is block of data
                if (chunk_remaining_bytes >= (valread - buffer_index)) { // rest of the buffer is data
                    if (*h.flags == 's') {
                        DEBUG_MESSAGE("--START FILENAME: ");
                        fprintf(f, "%.*s\n", (int)valread - buffer_index, cur);
                    } else if (*h.flags == 'e') {
                        DEBUG_MESSAGE("--END FILENAME: ");
                        fprintf(f, "%.*s\n", (int)valread - buffer_index, cur);
                    } else {
                        fprintf(f, "%.*s", (int)valread - buffer_index, cur);
                    }
                    chunk_remaining_bytes -= (valread - buffer_index);
                    buffer_index += (valread - buffer_index);

                    DEBUG_MESSAGE("\nENTIRE BUFFER USED\n");
                    DEBUG_MESSAGE("chunk_remaining: %d\n", chunk_remaining_bytes);
                    DEBUG_MESSAGE("buffer_index: %d\n", buffer_index);
                    DEBUG_MESSAGE("valread: %zd\n\n", valread);
                } else { // there is another header in the buffer somewhere
                    if (*h.flags == 's') {
                        DEBUG_MESSAGE("--START FILENAME: ");
                        fprintf(f, "%.*s", chunk_remaining_bytes, cur);
                    } else if (*h.flags == 'e') {
                        DEBUG_MESSAGE("--END FILENAME: ");
                        fprintf(f, "%.*s", chunk_remaining_bytes, cur);
                    } else {
                        fprintf(f, "%.*s", chunk_remaining_bytes, cur);
                    }
                    cur += chunk_remaining_bytes;
                    buffer_index += chunk_remaining_bytes; // == valread
                                                           //
                    chunk_remaining_bytes = 0;

                    DEBUG_MESSAGE("\nPARTIAL BUFFER USED\n");
                    DEBUG_MESSAGE("chunk_remaining: %d\n", chunk_remaining_bytes);
                    DEBUG_MESSAGE("buffer_index: %d\n", buffer_index);
                    DEBUG_MESSAGE("valread: %d\n\n", (int)valread);
                }

                if (chunk_remaining_bytes == 0) {
                    first = 1;
                }
            }
        }
    }

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    return 0;
}
