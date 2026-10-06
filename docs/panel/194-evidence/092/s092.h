/* Defect 092's shapes, lane ffi13, 2026-10-06, for panel 194: C functions that
   are handed a whole record and told how far to reach by another argument. */
#include <stdint.h>
#include <string.h>

struct one { int32_t a; };
struct pad { char c; int64_t d; };
struct outer { struct one inner; int32_t z; };
struct pfd { int32_t fd; int16_t events; int16_t revents; };

/* `void *` and a count in bytes: read(2), recv(2), memset(3). */
static inline int64_t fill(void *buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }

/* `const void *` and a count in bytes: write(2), send(2). */
static inline int64_t peek(const void *buf, uint64_t n) {
    const unsigned char *b = buf; int64_t s = 0;
    for (uint64_t i = 0; i < n; i++) s += b[i];
    return s;
}

/* The record's own pointer type and a count in bytes. */
static inline int64_t fill_typed(struct one *buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }

/* `void *` and a count C reads through a pointer: getsockopt(2)'s optlen. */
static inline int64_t fill_len(void *buf, uint32_t *n) { memset(buf, 0x41, (size_t)*n); return (int64_t)*n; }

/* The record's own pointer type and a count in RECORDS: poll(2), epoll_wait(2). */
static inline int32_t poll_like(struct pfd *fds, uint64_t nfds) {
    for (uint64_t i = 0; i < nfds; i++) fds[i].revents = 1;
    return (int32_t)nfds;
}
