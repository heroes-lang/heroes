/* Panel 178: is an all-zero pthread_mutex_t a usable mutex on this platform? */
#include <pthread.h>
#include <stdio.h>
#include <string.h>
int main(void) {
    pthread_mutex_t m;
    memset(&m, 0, sizeof m);
    pthread_mutex_t init = PTHREAD_MUTEX_INITIALIZER;
    printf("zero equals PTHREAD_MUTEX_INITIALIZER: %s\n", memcmp(&m, &init, sizeof m) == 0 ? "yes" : "no");
    int r = pthread_mutex_lock(&m);
    printf("lock on the zeroed mutex: %d (%s)\n", r, r ? strerror(r) : "ok");
    if (r == 0) printf("unlock: %d\n", pthread_mutex_unlock(&m));
    return 0;
}
