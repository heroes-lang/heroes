#ifndef FIXEDBUGS_092_RECORDS_H
#define FIXEDBUGS_092_RECORDS_H

/* Defect 092's shapes (lane b13-unit, 2026-10-06, from lane b12-ffi13's
   evidence for panel 194): C functions handed a whole record and told how far
   to reach by ANOTHER argument. Each reaches exactly as far as it is told. */
#include <stdint.h>
#include <string.h>

struct one { int32_t a; };
struct pad { char c; int64_t d; };
struct outer { struct one inner; int32_t z; };
struct pfd { int32_t fd; int16_t events; int16_t revents; };
typedef struct pfd *LPPFD;
typedef void *LPVOID;

/* `void *` and a count in bytes: read(2), recv(2), memset(3). */
static inline int64_t fill(void *buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }
static inline int64_t fill_pad(void *buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }
static inline int64_t fill_signed(void *buf, int64_t n) { if (n > 0) memset(buf, 0x41, (size_t)n); return n; }
static inline int64_t fill_lpvoid(LPVOID buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }
/* `const void *` and a count in bytes: write(2), send(2). */
static inline int64_t peek(const void *buf, uint64_t n) { const unsigned char *b = buf; int64_t s = 0; for (uint64_t i = 0; i < n; i++) s += b[i]; return s; }
/* `void *` and a count C reads through a pointer: getsockopt(2)'s optlen. */
static inline int64_t fill_len(void *buf, uint32_t *n) { memset(buf, 0x41, (size_t)*n); return (int64_t)*n; }
static inline int64_t fill_len64(void *buf, int64_t *n) { if (*n > 0) memset(buf, 0x41, (size_t)*n); return *n; }
/* The record's own pointer type and a count in RECORDS: poll(2), epoll_wait(2). */
static inline int32_t poll_like(struct pfd *fds, uint64_t nfds) { for (uint64_t i = 0; i < nfds; i++) fds[i].revents = 1; return (int32_t)nfds; }
static inline int32_t poll_typedef(LPPFD fds, uint64_t nfds) { for (uint64_t i = 0; i < nfds; i++) fds[i].revents = 1; return (int32_t)nfds; }
/* Two records, one count: memcpy(3). */
static inline int64_t copy_n(void *dst, const void *src, uint64_t n) { memcpy(dst, src, (size_t)n); return (int64_t)n; }

#endif
