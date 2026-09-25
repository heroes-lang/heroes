/* Panel 178 ffi-pragmatist: what a FORGOTTEN field costs when the rest is
   silently zero (R2, and R1 once `rest: zero` is written): hints with and
   without ai_socktype, counted results. */
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
static int count(int socktype) {
    struct addrinfo h; memset(&h, 0, sizeof h); h.ai_family = AF_INET; h.ai_socktype = socktype;
    struct addrinfo *r = NULL; int rc = getaddrinfo("localhost", "80", &h, &r); int n = 0;
    for (struct addrinfo *p = r; p; p = p->ai_next) n++;
    if (!rc) freeaddrinfo(r); return rc ? -rc : n;
}
int main(void) { printf("ai_socktype set: %d result(s); ai_socktype forgotten (zero): %d result(s)\n", count(SOCK_STREAM), count(0)); return 0; }
