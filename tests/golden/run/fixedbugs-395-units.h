#ifndef FIXEDBUGS_395_UNITS_H
#define FIXEDBUGS_395_UNITS_H

/* Defect 395's shapes (lane b13-unit, 2026-10-06): C functions told how far to
   reach in ANOTHER argument, counted in what their pointer points at. Each
   writes or reads exactly `n` of its unit, as `poll`, `mbstowcs` and every
   `T *p, size_t n` array API do. */
#include <stdint.h>
#include <string.h>
#include <wchar.h>

struct held { unsigned char buf[16]; int64_t after; };
struct ten { unsigned char buf[10]; int64_t after; };
struct pfd { int32_t fd; int16_t events; int16_t revents; };
typedef struct pfd *LPPFD;
typedef void *LPVOID;

#define HELD_BYTES ((uint64_t)16)
#define HELD_INTS ((uint64_t)4)

static inline int64_t fill_bytes(char *buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }
static inline int64_t fill_lpvoid(LPVOID buf, uint64_t n) { memset(buf, 0x41, (size_t)n); return (int64_t)n; }
static inline int64_t fill_ints(int *a, uint64_t n) { for (uint64_t k = 0; k < n; k++) a[k] = 0x41414141; return (int64_t)n; }
static inline int64_t read_ints(const int *a, uint64_t n) { int64_t s = 0; for (uint64_t k = 0; k < n; k++) s += a[k]; return s; }
static inline int64_t fill_shorts(int16_t *a, uint64_t n) { for (uint64_t k = 0; k < n; k++) a[k] = 0x4141; return (int64_t)n; }
static inline int32_t poll_like(struct pfd *fds, uint64_t nfds) { for (uint64_t i = 0; i < nfds; i++) fds[i].revents = 1; return (int32_t)nfds; }
static inline int32_t poll_typedef(LPPFD fds, uint64_t nfds) { for (uint64_t i = 0; i < nfds; i++) fds[i].revents = 1; return (int32_t)nfds; }
static inline int64_t fill_rows(int (*rows)[4], uint64_t n) { for (uint64_t k = 0; k < n; k++) rows[k][0] = 0x41414141; return (int64_t)n; }
static inline int64_t copy_n(void *dst, const void *src, uint64_t n) { memcpy(dst, src, (size_t)n); return (int64_t)n; }
static inline int64_t fill_len(void *buf, uint32_t *n) { memset(buf, 0x41, (size_t)*n); return (int64_t)*n; }

/* `mbstowcs`'s unit, `wchar_t`, four bytes on Darwin and Linux and two on
   Windows, spelled the same everywhere: the system's own declaration of
   `mbstowcs` is not, so a message quoting it would differ by leg. */
static inline int64_t fill_wide(wchar_t *dst, uint64_t n) { for (uint64_t k = 0; k < n; k++) dst[k] = L'A'; return (int64_t)n; }

#endif
