
#include <pthread.h>
#include <stdio.h>

typedef struct ClientThreadInfo {
    pthread_t thread_id;
    int client_fd;
    _Bool running;
} ClientThreadInfo;

void *run_server(void *keep_running_v);
