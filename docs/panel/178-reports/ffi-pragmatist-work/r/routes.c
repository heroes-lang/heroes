/* Panel 178 ffi-pragmatist: the C each route would EMIT, for real structs,
   against the real headers, under the emitter's own flag list
   (selfhost/cli/flags.hero: -std=gnu11 -Wall -Werror=... -fsigned-char), and
   each struct handed to the real call. The stack is dirtied before every
   construction so a byte a route fails to write shows up as 0xAA.
   R1  = `rest: zero`: memset of the CELL, then the named members stored in place.
   R0  = `T.zero()`, then `@` stores: the same memset, stores come later.
   A   = `[x; N]`: a compound literal with a GNU range designator for the array.
   T   = `s.to_fixed()`: a bounded memcpy of the str's bytes, the rest zero.
   L   = defect 091's repair: one guarded element store per byte. */
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <unistd.h>
#ifdef __APPLE__
#include <sys/mount.h>
#else
#include <sys/statvfs.h>
#endif

__attribute__((noinline)) static void dirty(void) { volatile unsigned char b[1 << 16]; for (size_t i = 0; i < sizeof b; i++) b[i] = 0xAA; }
static int count_aa(const void *p, size_t n) { const unsigned char *c = p; int k = 0; for (size_t i = 0; i < n; i++) k += c[i] == 0xAA; return k; }
static void panic(const char *m) { fprintf(stderr, "panic: %s\n", m); exit(134); }

/* a HeroStr stand-in: the runtime's {ptr, len}, NUL-terminated. */
typedef struct { const char *ptr; int64_t len; } Str;

/* ---- utsname ---------------------------------------------------------- */
__attribute__((noinline)) static int uname_R1(void) { struct utsname u; memset(&u, 0, sizeof u); int r = uname(&u); printf("  R1 uname=%d sysname=%s machine=%s\n", r, u.sysname, u.machine); return r; }
__attribute__((noinline)) static int uname_A(void) {
    struct utsname u;
    int8_t z = 0; /* A's element is a Heroes value, so the emitter names a temporary */
    u = (struct utsname){.sysname = {[0 ... sizeof u.sysname - 1] = z}, .nodename = {[0 ... sizeof u.nodename - 1] = z},
                         .release = {[0 ... sizeof u.release - 1] = z}, .version = {[0 ... sizeof u.version - 1] = z},
                         .machine = {[0 ... sizeof u.machine - 1] = z}};
    int r = uname(&u); printf("  A  uname=%d sysname=%s machine=%s\n", r, u.sysname, u.machine); return r; }

/* ---- sockaddr_un: bind, listen, connect, getsockname -------------------- */
static int roundtrip(const char *route, struct sockaddr_un *a, const char *path) {
    unlink(path);
    int s = socket(AF_UNIX, SOCK_STREAM, 0), c = socket(AF_UNIX, SOCK_STREAM, 0);
    int b = bind(s, (const struct sockaddr *)a, (socklen_t)sizeof *a), l = listen(s, 1);
    int k = connect(c, (const struct sockaddr *)a, (socklen_t)sizeof *a);
    struct sockaddr_un back; memset(&back, 0, sizeof back); socklen_t n = sizeof back;
    int g = getsockname(s, (struct sockaddr *)&back, &n);
    struct sockaddr_storage ss; memset(&ss, 0, sizeof ss); socklen_t m = sizeof ss;
    int g2 = getsockname(c, (struct sockaddr *)&ss, &m);
    printf("  %-3s bind=%d listen=%d connect=%d getsockname=%d path=%s | storage getsockname=%d family=%d, 0xAA left in addr: %d\n",
           route, b, l, k, g, back.sun_path, g2, (int)ss.ss_family, count_aa(a, sizeof *a));
    close(c); close(s); unlink(path);
    return b | l | k | g | g2;
}
__attribute__((noinline)) static int sun_R1_L(Str p) {
    struct sockaddr_un a; memset(&a, 0, sizeof a); a.sun_family = AF_UNIX;              /* R1 */
    for (int64_t i = 0; i < p.len; i++)                                                   /* L  */
        a.sun_path[((uint64_t)(i) >= (uint64_t)sizeof a.sun_path ? (panic("index out of range for a fixed array"), (int64_t)0) : (i))] = (int8_t)p.ptr[i];
    return roundtrip("R1L", &a, p.ptr); }
__attribute__((noinline)) static int sun_R1_T(Str p) {
    struct sockaddr_un a; memset(&a, 0, sizeof a); a.sun_family = AF_UNIX;              /* R1 */
    if ((uint64_t)p.len >= sizeof a.sun_path) panic("to_fixed: does not fit");        /* T: bytes + a terminating zero */
    memcpy(a.sun_path, p.ptr, (size_t)p.len); memset(a.sun_path + p.len, 0, sizeof a.sun_path - (size_t)p.len);
    return roundtrip("R1T", &a, p.ptr); }
__attribute__((noinline)) static int sun_A_L(Str p) {
    struct sockaddr_un a; int8_t z = 0;
    a = (struct sockaddr_un){.sun_family = AF_UNIX, .sun_path = {[0 ... sizeof a.sun_path - 1] = z}};   /* A */
    for (int64_t i = 0; i < p.len; i++) a.sun_path[((uint64_t)(i) >= (uint64_t)sizeof a.sun_path ? (panic("index out of range for a fixed array"), (int64_t)0) : (i))] = (int8_t)p.ptr[i];
    return roundtrip("AL", &a, p.ptr); }

/* ---- addrinfo hints ----------------------------------------------------- */
__attribute__((noinline)) static int ai_R1(void) {
    struct addrinfo h; memset(&h, 0, sizeof h); h.ai_family = AF_INET; h.ai_socktype = SOCK_STREAM;
    int aa = count_aa(&h, sizeof h); struct addrinfo *r = NULL; int rc = getaddrinfo("localhost", "80", &h, &r);
    printf("  R1 getaddrinfo=%d, 0xAA left in hints: %d\n", rc, aa); if (!rc) freeaddrinfo(r); return rc; }
__attribute__((noinline)) static int ai_S(void) {  /* today's emitter shape, the `partial` construction */
    struct addrinfo h; int32_t t1 = AF_INET, t2 = SOCK_STREAM; h = (struct addrinfo){.ai_family = t1, .ai_socktype = t2};
    int aa = count_aa(&h, sizeof h); struct addrinfo *r = NULL; int rc = getaddrinfo("localhost", "80", &h, &r);
    printf("  S  getaddrinfo=%d, 0xAA left in hints: %d\n", rc, aa); if (!rc) freeaddrinfo(r); return rc; }
__attribute__((noinline)) static int ai_garbage(void) {  /* what the vetoed uninitialised route would hand over */
    struct addrinfo h; memset(&h, 0xAA, sizeof h); h.ai_family = AF_INET; h.ai_socktype = SOCK_STREAM; h.ai_flags = 0; h.ai_protocol = 0;
    struct addrinfo *r = NULL; int rc = getaddrinfo("localhost", "80", &h, &r);
    printf("  garbage in ai_addrlen/ai_canonname/ai_addr/ai_next: getaddrinfo=%d (%s)\n", rc, rc ? gai_strerror(rc) : "ok"); if (!rc) freeaddrinfo(r); return 0; }

/* ---- statfs / statvfs --------------------------------------------------- */
__attribute__((noinline)) static int fs_R1(void) {
#ifdef __APPLE__
    struct statfs s; memset(&s, 0, sizeof s); int r = statfs("/", &s); printf("  R1 statfs=%d type=%s on=%s\n", r, s.f_fstypename, s.f_mntonname);
#else
    struct statvfs s; memset(&s, 0, sizeof s); int r = statvfs("/", &s); printf("  R1 statvfs=%d bsize=%lu namemax=%lu\n", r, (unsigned long)s.f_bsize, (unsigned long)s.f_namemax);
#endif
    return r; }

/* ---- pthread_mutex_t: zero, and zero-then-init -------------------------- */
__attribute__((noinline)) static int mx_R0(void) {
    pthread_mutex_t m; memset(&m, 0, sizeof m); int l = pthread_mutex_lock(&m);
    printf("  R0 zero then lock=%d%s%s\n", l, l ? " " : "", l ? strerror(l) : ""); if (!l) pthread_mutex_unlock(&m);
    pthread_mutex_t n; memset(&n, 0, sizeof n); int i = pthread_mutex_init(&n, NULL); int l2 = pthread_mutex_lock(&n);
    printf("  R0 zero then init=%d then lock=%d\n", i, l2); if (!l2) pthread_mutex_unlock(&n); return 0; }

int main(void) {
    const char *ascii = "/tmp/heroes-178-r.sock", *utf8 = "/tmp/h\xc3\xa8roes-178-r.sock";
    Str pa = {ascii, (int64_t)strlen(ascii)}, pu = {utf8, (int64_t)strlen(utf8)};
    printf("utsname (%zu bytes):\n", sizeof(struct utsname)); dirty(); uname_R1(); dirty(); uname_A();
    printf("sockaddr_un (%zu bytes, sun_path %zu):\n", sizeof(struct sockaddr_un), sizeof(((struct sockaddr_un *)0)->sun_path));
    dirty(); sun_R1_L(pa); dirty(); sun_R1_T(pa); dirty(); sun_A_L(pa);
    printf(" the UTF-8 path, %lld bytes:\n", (long long)pu.len); dirty(); sun_R1_L(pu); dirty(); sun_R1_T(pu);
    printf("addrinfo hints (%zu bytes):\n", sizeof(struct addrinfo)); dirty(); ai_S(); dirty(); ai_R1(); ai_garbage();
    printf("filesystem:\n"); dirty(); fs_R1();
    printf("pthread_mutex_t (%zu bytes):\n", sizeof(pthread_mutex_t)); dirty(); mx_R0();
    return 0;
}
