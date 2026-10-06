/* Panel 178 ffi-pragmatist: the instrument padding.c lacked at -O2.
   MemorySanitizer tracks every byte's initialisedness, padding included, and
   its write() interceptor reports a buffer holding any uninitialised byte. So
   each shape builds `struct addrinfo` (4 bytes of padding on LP64) exactly as a
   route would emit it and hands &cell to write(): a report means bytes nobody
   defined crossed into C. One child per shape; exit 77 is MSan's verdict. */
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
static int fd;
static volatile int32_t fam = AF_INET, sock = SOCK_STREAM;   /* values the optimiser cannot see */
#define CROSS(x) (void)!write(fd, &(x), sizeof(x))

/* today's emitter, verbatim shape (p/s/addrinfo_S.c:282-284): literal into a
   temporary, temporary into the cell, &cell to C */
__attribute__((noinline)) static void s_today(void) { struct addrinfo t21, h2_h; int32_t t10 = fam, t20 = sock;
    t21 = (struct addrinfo){.ai_family = t10, .ai_socktype = t20}; h2_h = t21; CROSS(h2_h); }
/* R1 as a compound literal straight into the cell (no temporary) */
__attribute__((noinline)) static void r1_literal(void) { struct addrinfo h; h = (struct addrinfo){.ai_family = fam, .ai_socktype = sock}; CROSS(h); }
/* R1 as memset of the CELL, then member stores in place */
__attribute__((noinline)) static void r1_memset_cell(void) { struct addrinfo h; memset(&h, 0, sizeof h); h.ai_family = fam; h.ai_socktype = sock; CROSS(h); }
/* R1 as memset of a TEMPORARY, stores, then the emitter's copy into the cell */
__attribute__((noinline)) static void r1_memset_temp_copy(void) { struct addrinfo t, h; memset(&t, 0, sizeof t); t.ai_family = fam; t.ai_socktype = sock; h = t; CROSS(h); }
/* `= {0}` at the declaration, then stores */
__attribute__((noinline)) static void brace_zero(void) { struct addrinfo h = {0}; h.ai_family = fam; h.ai_socktype = sock; CROSS(h); }
/* R0 as `(T){0}`, then `@` stores */
__attribute__((noinline)) static void r0_literal_then_stores(void) { struct addrinfo h; h = (struct addrinfo){0}; h.ai_family = fam; h.ai_socktype = sock; CROSS(h); }
/* A union whose first member is smaller than the union: union sigval */
__attribute__((noinline)) static void union_brace_zero(void) { union sigval v; v = (union sigval){0}; CROSS(v); }
__attribute__((noinline)) static void union_memset(void) { union sigval v; memset(&v, 0, sizeof v); CROSS(v); }
/* termios: padding after c_cc on Linux */
__attribute__((noinline)) static void termios_literal(void) { struct termios t; t = (struct termios){.c_lflag = (tcflag_t)fam}; CROSS(t); }
__attribute__((noinline)) static void termios_memset(void) { struct termios t; memset(&t, 0, sizeof t); t.c_lflag = (tcflag_t)fam; CROSS(t); }

/* CONTROL: every field assigned, no initialiser: the 4 padding bytes are
   never written, so an instrument that measures anything must report here. */
__attribute__((noinline)) static void control(void) { struct addrinfo h; h.ai_flags = 0; h.ai_family = fam; h.ai_socktype = sock; h.ai_protocol = 0;
    h.ai_addrlen = 0; h.ai_canonname = 0; h.ai_addr = 0; h.ai_next = 0; CROSS(h); }
/* CONTROL 2: the control's value COPIED into a memset cell: does a struct copy carry undefined padding over defined bytes? */
__attribute__((noinline)) static void control_copied_over_zero(void) { struct addrinfo h, c; memset(&c, 0, sizeof c); h.ai_flags = 0; h.ai_family = fam; h.ai_socktype = sock; h.ai_protocol = 0;
    h.ai_addrlen = 0; h.ai_canonname = 0; h.ai_addr = 0; h.ai_next = 0; c = h; CROSS(c); }
/* CONTROL 3: union written through its small first member only */
__attribute__((noinline)) static void union_small_member(void) { union sigval v; v.sival_int = fam; CROSS(v); }
static void run(const char *name, void (*f)(void)) {
    fflush(stdout); pid_t p = fork();
    if (p == 0) { f(); _exit(0); }
    int st; waitpid(p, &st, 0);
    printf("%-40s %s\n", name, WIFEXITED(st) && WEXITSTATUS(st) == 0 ? "clean: every byte defined" : WIFEXITED(st) && WEXITSTATUS(st) == 77 ? "MSan: UNINITIALISED bytes reached write()" : "other");
}
int main(void) {
    fd = open("/dev/null", O_WRONLY);
    printf("sizeof addrinfo %zu, union sigval %zu (first member %zu), termios %zu\n", sizeof(struct addrinfo), sizeof(union sigval), sizeof(int), sizeof(struct termios));
    run("CONTROL fields only, no initialiser", control);
    run("CONTROL copied over a zeroed cell", control_copied_over_zero);
    run("CONTROL union via its 4-byte member", union_small_member);
    run("S today: literal -> temp -> cell", s_today);
    run("R1 literal straight into cell", r1_literal);
    run("R1 memset cell, stores in place", r1_memset_cell);
    run("R1 memset temp, stores, copy to cell", r1_memset_temp_copy);
    run("= {0}, stores", brace_zero);
    run("R0 (T){0}, stores", r0_literal_then_stores);
    run("union sigval = (union){0}", union_brace_zero);
    run("union sigval memset", union_memset);
    run("termios literal", termios_literal);
    run("termios memset", termios_memset);
    return 0;
}
