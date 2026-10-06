/* Panel 178: do padding bytes come out zero in the shape the emitter writes today,
   `T t; ... t = (T){.field = v};` (see --emit-c of ffi-a-filled-record-is-asked-for),
   against memset and against `T t = {0};`? The stack is dirtied with 0xAA first. */
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <termios.h>
__attribute__((noinline)) static void dirty(void) { volatile unsigned char b[4096]; for (int i = 0; i < 4096; i++) b[i] = 0xAA; }
static int count_aa(const void *p, size_t n) { const unsigned char *c = p; int k = 0; for (size_t i = 0; i < n; i++) k += c[i] == 0xAA; return k; }
__attribute__((noinline)) static int emitter_shape_addrinfo(void) { struct addrinfo t; t = (struct addrinfo){.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM}; return count_aa(&t, sizeof t); }
__attribute__((noinline)) static int memset_addrinfo(void) { struct addrinfo t; memset(&t, 0, sizeof t); t.ai_family = AF_UNSPEC; t.ai_socktype = SOCK_STREAM; return count_aa(&t, sizeof t); }
__attribute__((noinline)) static int brace_zero_addrinfo(void) { struct addrinfo t = {0}; t.ai_family = AF_UNSPEC; t.ai_socktype = SOCK_STREAM; return count_aa(&t, sizeof t); }
__attribute__((noinline)) static int emitter_shape_termios(void) { struct termios t; t = (struct termios){.c_lflag = 1}; return count_aa(&t, sizeof t); }
__attribute__((noinline)) static int memset_termios(void) { struct termios t; memset(&t, 0, sizeof t); t.c_lflag = 1; return count_aa(&t, sizeof t); }
__attribute__((noinline)) static int control_addrinfo(void) { struct addrinfo t; t.ai_flags = 0; t.ai_family = AF_UNSPEC; t.ai_socktype = SOCK_STREAM; t.ai_protocol = 0; t.ai_addrlen = 0; t.ai_canonname = 0; t.ai_addr = 0; t.ai_next = 0; return count_aa(&t, sizeof t); }
int main(void) {
    size_t fields = sizeof(int) * 4 + sizeof(socklen_t) + sizeof(char *) + sizeof(struct sockaddr *) + sizeof(struct addrinfo *);
    printf("addrinfo: sizeof %zu, sum of fields %zu, padding bytes %zu\n", sizeof(struct addrinfo), fields, sizeof(struct addrinfo) - fields);
    dirty(); printf("  CONTROL, every field assigned, no initialiser, 0xAA bytes left: %d\n", control_addrinfo());
    dirty(); printf("  emitter shape, 0xAA bytes left: %d\n", emitter_shape_addrinfo());
    dirty(); printf("  memset,        0xAA bytes left: %d\n", memset_addrinfo());
    dirty(); printf("  = {0},         0xAA bytes left: %d\n", brace_zero_addrinfo());
    printf("termios: sizeof %zu, c_cc at %zu, c_ispeed at %zu\n", sizeof(struct termios), offsetof(struct termios, c_cc), offsetof(struct termios, c_ispeed));
    dirty(); printf("  emitter shape, 0xAA bytes left: %d\n", emitter_shape_termios());
    dirty(); printf("  memset,        0xAA bytes left: %d\n", memset_termios());
    return 0;
}
