#include <pthread.h>
#include <stdio.h>
#include <string.h>
int main(void) {
    pthread_spinlock_t s; memset(&s, 0xAA, sizeof s);
    int r = pthread_spin_init(&s, 0);
    printf("spin_init=%d, the unlocked value is %d, sizeof %zu\n", r, (int)s, sizeof s);
    pthread_spinlock_t z = 0; int t = pthread_spin_trylock(&z);
    printf("trylock(zero)=%d (%s)\n", t, t ? strerror(t) : "ok");
    pthread_barrier_t b; memset(&b, 0, sizeof b);
    printf("barrier sizeof %zu\n", sizeof b);
    return 0;
}
