/* Panel 178 ffi-pragmatist: for every census type the headers give a
   constructor (a *_init / *_Init / sigemptyset / FD_ZERO / *_INITIALIZER /
   *_INIT), is the constructed value all zeros on THIS platform, and what does
   a program get if it hands the all-zero value to the library instead?
   Built: clang -std=gnu11 -O0 zinit.c -lpthread -lcrypto */
#define OPENSSL_SUPPRESS_DEPRECATED 1
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <wchar.h>
#include <sys/wait.h>

static int allzero(const void *p, size_t n) { const unsigned char *c = p; for (size_t i = 0; i < n; i++) if (c[i]) return 0; return 1; }
#define ROW(name, zeq, note) printf("%-22s ctor==zero:%-4s %s\n", name, (zeq) ? "yes" : "NO", note)
static char buf[256];
static const char *rc(int r) { snprintf(buf, sizeof buf, "%d%s%s", r, r ? " " : "", r ? strerror(r) : ""); return buf; }
static int ran;
static void once_fn(void) { ran++; }
static void *thr(void *a) { (void)a; return 0; }

static char note[512];
/* Each probe runs in a child with a 3-second alarm, because a zero value handed
   to a blocking primitive may never return: that is itself the measurement. */
#define PROBE(label, ...) do { fflush(stdout); pid_t p_ = fork(); if (p_ == 0) { alarm(3); __VA_ARGS__; fflush(stdout); _exit(0); } \
    int st_; waitpid(p_, &st_, 0); if (WIFSIGNALED(st_)) printf("%-22s KILLED by signal %d (%s) -- the zero value %s\n", label, WTERMSIG(st_), strsignal(WTERMSIG(st_)), WTERMSIG(st_) == SIGALRM ? "BLOCKED FOREVER" : "crashed"); } while (0)
int main(void) {
    PROBE("pthread_mutex_t", { pthread_mutex_t c, z; memset(&c, 0xAA, sizeof c); pthread_mutex_init(&c, NULL); memset(&z, 0, sizeof z);
      pthread_mutex_t s = PTHREAD_MUTEX_INITIALIZER; int r = pthread_mutex_lock(&z);
      snprintf(note, sizeof note, "INITIALIZER all-zero:%s; lock(zero)=%s", allzero(&s, sizeof s) ? "yes" : "NO", rc(r)); if (!r) pthread_mutex_unlock(&z);
      ROW("pthread_mutex_t", allzero(&c, sizeof c), note); });
    PROBE("pthread_cond_t", { pthread_cond_t c, z; memset(&c, 0xAA, sizeof c); pthread_cond_init(&c, NULL); memset(&z, 0, sizeof z);
      pthread_cond_t s = PTHREAD_COND_INITIALIZER; int r1 = pthread_cond_signal(&z); int r2 = pthread_cond_broadcast(&z);
      pthread_mutex_t m; pthread_mutex_init(&m, NULL); pthread_mutex_lock(&m); struct timespec ts = {0, 0};
      int r3 = pthread_cond_timedwait(&z, &m, &ts); pthread_mutex_unlock(&m);
      snprintf(note, sizeof note, "INITIALIZER all-zero:%s; signal(zero)=%s", allzero(&s, sizeof s) ? "yes" : "NO", rc(r1));
      snprintf(note + strlen(note), sizeof note - strlen(note), "; broadcast=%s", rc(r2));
      snprintf(note + strlen(note), sizeof note - strlen(note), "; timedwait=%s", rc(r3));
      ROW("pthread_cond_t", allzero(&c, sizeof c), note); });
    PROBE("pthread_rwlock_t", { pthread_rwlock_t c, z; memset(&c, 0xAA, sizeof c); pthread_rwlock_init(&c, NULL); memset(&z, 0, sizeof z);
      pthread_rwlock_t s = PTHREAD_RWLOCK_INITIALIZER; int r = pthread_rwlock_wrlock(&z);
      snprintf(note, sizeof note, "INITIALIZER all-zero:%s; wrlock(zero)=%s", allzero(&s, sizeof s) ? "yes" : "NO", rc(r)); if (!r) pthread_rwlock_unlock(&z);
      ROW("pthread_rwlock_t", allzero(&c, sizeof c), note); });
    PROBE("pthread_once_t INIT", { pthread_once_t s = PTHREAD_ONCE_INIT, z; memset(&z, 0, sizeof z); ran = 0; int r = pthread_once(&z, once_fn); int r2 = pthread_once(&z, once_fn);
      snprintf(note, sizeof note, "(no ctor fn) once(zero) twice=%d,%d, routine ran %d time(s)", r, r2, ran);
      ROW("pthread_once_t INIT", allzero(&s, sizeof s), note); });
    PROBE("pthread_attr_t", { pthread_attr_t c, z; memset(&c, 0xAA, sizeof c); pthread_attr_init(&c); memset(&z, 0, sizeof z); pthread_t t;
      int r = pthread_create(&t, &z, thr, NULL); if (!r) pthread_join(t, NULL);
      snprintf(note, sizeof note, "create(attr=zero)=%s", rc(r)); ROW("pthread_attr_t", allzero(&c, sizeof c), note); });
    PROBE("pthread_mutexattr_t", { pthread_mutexattr_t c, z; memset(&c, 0xAA, sizeof c); pthread_mutexattr_init(&c); memset(&z, 0, sizeof z); pthread_mutex_t m;
      int r = pthread_mutex_init(&m, &z); snprintf(note, sizeof note, "mutex_init(attr=zero)=%s", rc(r));
      if (!r) { int l = pthread_mutex_lock(&m); snprintf(note + strlen(note), sizeof note - strlen(note), ", then lock=%s", rc(l)); }
      ROW("pthread_mutexattr_t", allzero(&c, sizeof c), note); });
    PROBE("pthread_condattr_t", { pthread_condattr_t c, z; memset(&c, 0xAA, sizeof c); pthread_condattr_init(&c); memset(&z, 0, sizeof z); pthread_cond_t cv;
      int r = pthread_cond_init(&cv, &z); snprintf(note, sizeof note, "cond_init(attr=zero)=%s", rc(r)); ROW("pthread_condattr_t", allzero(&c, sizeof c), note); });
    PROBE("pthread_rwlockattr_t", { pthread_rwlockattr_t c, z; memset(&c, 0xAA, sizeof c); pthread_rwlockattr_init(&c); memset(&z, 0, sizeof z); pthread_rwlock_t rw;
      int r = pthread_rwlock_init(&rw, &z); snprintf(note, sizeof note, "rwlock_init(attr=zero)=%s", rc(r)); ROW("pthread_rwlockattr_t", allzero(&c, sizeof c), note); });
    PROBE("sigset_t", { sigset_t c; memset(&c, 0xAA, sizeof c); sigemptyset(&c); sigset_t z; memset(&z, 0, sizeof z);
      snprintf(note, sizeof note, "sigismember(zero,SIGINT)=%d", sigismember(&z, SIGINT)); ROW("sigset_t", allzero(&c, sizeof c), note); });
    PROBE("fd_set FD_ZERO", { fd_set c; memset(&c, 0xAA, sizeof c); FD_ZERO(&c); ROW("fd_set FD_ZERO", allzero(&c, sizeof c), ""); });
    PROBE("in6_addr ANY_INIT", { struct in6_addr a = IN6ADDR_ANY_INIT; ROW("in6_addr ANY_INIT", allzero(&a, sizeof a), ""); });
    PROBE("mbstate_t", { mbstate_t z; memset(&z, 0, sizeof z); setlocale(LC_ALL, "C.UTF-8"); if (!setlocale(LC_ALL, NULL) || strcmp(setlocale(LC_ALL, NULL), "C.UTF-8")) setlocale(LC_ALL, "en_US.UTF-8");
      wchar_t w = 0; size_t n = mbrtowc(&w, "\xc3\xa8", 2, &z);
      snprintf(note, sizeof note, "mbrtowc(zero, e-grave)=%zu, U+%04X, locale %s", n, (unsigned)w, setlocale(LC_ALL, NULL));
      ROW("mbstate_t", 1, note); });
    PROBE("SHA256_CTX", { static const unsigned char want[32] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
      SHA256_CTX c, z; memset(&c, 0xAA, sizeof c); SHA256_Init(&c); memset(&z, 0, sizeof z); unsigned char d[32]; memset(d, 0x55, sizeof d);
      int u = SHA256_Update(&z, "abc", 3); int f = SHA256_Final(d, &z);
      snprintf(note, sizeof note, "Update(zero)=%d Final=%d, digest of \"abc\" %s; bytes Final wrote into a 0x55-filled buffer: %d of 32", u, f, memcmp(d, want, 32) ? "WRONG" : "right", 32 - (int)(({ int k_ = 0; for (int i_ = 0; i_ < 32; i_++) k_ += d[i_] == 0x55; k_; })));
      ROW("SHA256_CTX", allzero(&c, sizeof c), note); });
    PROBE("SHA_CTX", { SHA_CTX c; memset(&c, 0xAA, sizeof c); SHA1_Init(&c); ROW("SHA_CTX", allzero(&c, sizeof c), ""); });
    PROBE("SHA512_CTX", { SHA512_CTX c; memset(&c, 0xAA, sizeof c); SHA512_Init(&c); ROW("SHA512_CTX", allzero(&c, sizeof c), ""); });
    PROBE("struct flock (no ctor)", { char path[] = "/tmp/heroes-178-flockXXXXXX"; int fd = mkstemp(path); struct flock z; memset(&z, 0, sizeof z);
      int r = fcntl(fd, F_SETLK, &z); int e = errno;
      snprintf(note, sizeof note, "F_RDLCK=%d F_WRLCK=%d F_UNLCK=%d; fcntl(F_SETLK, zero)=%d%s%s", F_RDLCK, F_WRLCK, F_UNLCK, r, r ? " " : " (a lock was TAKEN)", r ? strerror(e) : "");
      ROW("struct flock (no ctor)", 0, note); close(fd); unlink(path); });
    PROBE("struct sigevent", { printf("%-22s SIGEV_NONE=%d SIGEV_SIGNAL=%d SIGEV_THREAD=%d\n", "struct sigevent", SIGEV_NONE, SIGEV_SIGNAL, SIGEV_THREAD); });
    PROBE("struct sigaction", { printf("%-22s SIG_DFL is null: %s\n", "struct sigaction", SIG_DFL == (void (*)(int))0 ? "yes" : "no"); });
#ifdef __linux__
    PROBE("pthread_barrier_t", { pthread_barrier_t z; memset(&z, 0, sizeof z); pthread_barrier_t c; memset(&c, 0xAA, sizeof c); pthread_barrier_init(&c, NULL, 1);
      int r = pthread_barrier_wait(&z); snprintf(note, sizeof note, "wait(zero)=%d (PTHREAD_BARRIER_SERIAL_THREAD=%d)", r, PTHREAD_BARRIER_SERIAL_THREAD);
      ROW("pthread_barrier_t", allzero(&c, sizeof c), note); });
    PROBE("pthread_spinlock_t", { pthread_spinlock_t z = 0, c; memset(&c, 0xAA, sizeof c); pthread_spin_init(&c, 0); int r = pthread_spin_lock(&z);
      snprintf(note, sizeof note, "lock(zero)=%s", rc(r)); ROW("pthread_spinlock_t", allzero(&c, sizeof c), note); });
#endif
    return 0;
}
