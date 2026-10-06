/* Panel 178, compiler-engineer: dirty the stack, and count the padding bytes of a
   struct addrinfo that are not zero (4 of them on every leg, between ai_addrlen and ai_canonname). */
#include <netdb.h>
#include <stddef.h>
#include <string.h>
__attribute__((noinline)) static int dirty_stack(void) { volatile unsigned char b[8192]; for (int i = 0; i < 8192; i++) b[i] = 0xAA; return b[100]; }
__attribute__((noinline)) static int padding_nonzero(const struct addrinfo *p) {
    const unsigned char *c = (const unsigned char *)p;
    size_t from = offsetof(struct addrinfo, ai_addrlen) + sizeof p->ai_addrlen, to = offsetof(struct addrinfo, ai_canonname);
    int k = 0; for (size_t i = from; i < to; i++) k += c[i] != 0; return k;
}
