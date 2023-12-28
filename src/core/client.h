
#include <stdio.h>

int client_send_file(const char *file_name);
int read_chunk(FILE *fp, char *buf, int s);
