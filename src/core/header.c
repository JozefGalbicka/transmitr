
#include "header.h"
#include <netinet/in.h> // OR #include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

unsigned char *serialize_header(struct Header *self, unsigned char buffer[16]) {
    unsigned char *orig = buffer;

    memcpy(buffer, self->type, 10);
    memcpy(buffer + 10, self->flags, 2);

    uint32_t len = htonl(self->data_length);
    // uint32_t len = h->data_length;
    memcpy(buffer + 12, &len, 4);

    return orig;
}

unsigned char *deserialize_header(struct Header *self,
                                  unsigned char buffer[16]) {
    unsigned char *orig = buffer;

    memcpy(self->type, buffer, 10);
    memcpy(self->flags, buffer + 10, 2);

    uint32_t len = htonl(self->data_length);
    memcpy(&len, buffer + 12, 4);
    self->data_length = ntohl(len);

    return orig;
}

void header_print(Header* self) {
    printf("TYPE: %.*s\n", 10, self->type);
    printf("FLAGS: %.*s\n", 2, self->flags);
    printf("LENGTH: %d\n", self->data_length);
}
