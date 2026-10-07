#ifndef FFI_COUNTED_BY_A_STATED_EXTENT_H
#define FFI_COUNTED_BY_A_STATED_EXTENT_H

/* Panel 196's R5 (lane b13-land-buf, 2026-10-07): C functions whose extent is
   FIXED and carried by no argument, as `SHA256_Final`'s 32 bytes are. Each
   writes exactly the number its name says, in what its pointer points at. */
#include <stdint.h>
#include <string.h>

struct held { unsigned char buf[16]; int64_t after; };
struct pair { int32_t a; int32_t b; };

#define HELD_LEN 16

static inline void fill16(unsigned char *p) { memset(p, 0x41, 16); }
static inline void fill16_const(unsigned char *p) { memset(p, 0x42, HELD_LEN); }
static inline void fill16_under(unsigned char *p) { memset(p, 0x43, 16); }
static inline void fill16_hex(void *p) { memset(p, 0x44, 16); }
static inline void fill_ints4(int32_t *p) { for (int k = 0; k < 4; k++) p[k] = 0x01010101 * (k + 1); }
static inline void fill_pair(struct pair *p) { p->a = 7; p->b = 8; }

#endif
