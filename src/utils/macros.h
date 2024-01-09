#define DEBUG 1

#if DEBUG
#define DEBUG_MESSAGE(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_MESSAGE(fmt, ...)
#endif
