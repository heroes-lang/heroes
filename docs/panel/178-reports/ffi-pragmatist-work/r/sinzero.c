/* Panel 178 ffi-pragmatist: does a kernel compare bytes of a struct the program
   never meant to write? `sin_zero` is a named field nobody sets; bind() to a
   specific local address with it zero, and with it 0xAA. */
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
static void try(const char *label, unsigned char fill) {
    struct sockaddr_in a; memset(&a, 0, sizeof a);
    a.sin_family = AF_INET; a.sin_port = 0; inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    memset(a.sin_zero, fill, sizeof a.sin_zero);
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int r = bind(s, (struct sockaddr *)&a, sizeof a); int e = errno;
    printf("%-26s bind(127.0.0.1)=%d%s%s\n", label, r, r ? " " : "", r ? strerror(e) : "");
    close(s);
}
int main(void) { try("sin_zero all zero:", 0); try("sin_zero 0xAA (garbage):", 0xAA); return 0; }
