#ifndef FFI_A_STATED_EXTENT_PAST_THE_PLACE_H
#define FFI_A_STATED_EXTENT_PAST_THE_PLACE_H

/* Panel 196's R5 (lane b13-land-buf, 2026-10-07): C functions whose extent is
   fixed and carried by no argument, each lent a place shorter than it. */
#include <stdint.h>
#include <string.h>

struct short_one { unsigned char buf[16]; int64_t after; };
struct small { int32_t a; };

#define DIGEST_LEN 32

static inline void digest32(unsigned char *p) { memset(p, 0x41, 32); }
static inline void digest_const(unsigned char *p) { memset(p, 0x41, DIGEST_LEN); }
static inline void fill_ints8(int32_t *p) { for (int k = 0; k < 8; k++) p[k] = k; }
static inline void two_smalls(struct small *p) { p[0].a = 1; p[1].a = 2; }

#endif
