#include <pthread.h>
static inline void px_exit(void) { pthread_exit(NULL); }
