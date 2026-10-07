#ifndef FIXEDBUGS_396_A_BUFFER_OF_ANOTHER_ELEMENT_H
#define FIXEDBUGS_396_A_BUFFER_OF_ANOTHER_ELEMENT_H

/* Defect 396's buffers against pointers that point at something else (lane
   b13-land-buf, 2026-10-07; panel 196's R4). */
#include <stdint.h>
#include <string.h>

struct pair { int32_t a; int32_t b; };

static inline void wide(unsigned char *md) { memset(md, 1, 8); }
static inline void ints_void(void *p) { memset(p, 1, 16); }
static inline void pairs_as_ints(int *p) { p[0] = 1; p[1] = 2; }
static inline void signed_chars(char *p) { memset(p, 1, 4); }
static inline void bytes_void(void *p) { memset(p, 1, 4); }
static inline void plain_chars(char *p) { memset(p, 1, 4); }

#endif
