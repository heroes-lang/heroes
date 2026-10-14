/* Critic, panel 178: the CONTROL inside the same binary. Every field stored,
   no initialiser: the 4 padding bytes are never written, so MSan must report. */
#include <netdb.h>
#include <unistd.h>
static volatile int ctl_v = 1;
__attribute__((noinline)) static long ctl_write(void) {
    struct addrinfo h; h.ai_flags = 0; h.ai_family = ctl_v; h.ai_socktype = ctl_v; h.ai_protocol = 0;
    h.ai_addrlen = 0; h.ai_canonname = 0; h.ai_addr = 0; h.ai_next = 0;
    return (long)write(1, &h, sizeof h);
}
