
#include <stddef.h>
#include <stdint.h>

typedef struct Header {
    char type[10];
    char flags[2];
    uint32_t data_length;
    // char* data;
} Header;

// Note: unsigned char[16] indicates that the size of a buffer should be at
// least 16
unsigned char *serialize_header(struct Header *self, unsigned char buffer[16]);
unsigned char *deserialize_header(struct Header *self, unsigned char buffer[16]);
void header_print(Header *self);
