
#include "../structures/list/array_list.h"
#include <stdio.h>

typedef struct Client {
    struct ArrayList *servers;
} Client;

void client_init(Client *self);
void client_destroy(Client *self);

int client_connect(Client *self, const char *ip_addr);
void client_close_all_connections(Client *self);


void client_send_file_to_all(Client *self, const char *path, const char mode);
int client_send_file(int client_fd, const char *path, const char mode);
