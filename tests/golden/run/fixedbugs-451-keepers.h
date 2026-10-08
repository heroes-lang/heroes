#ifndef FIXEDBUGS_451_KEEPERS_H
#define FIXEDBUGS_451_KEEPERS_H

/* Defect 451's shapes on a header of the program's own: C that keeps the
   address of what it was lent and reaches it after the call, as a library
   that holds a pointer it was handed for one call would (an error buffer kept
   by a handle, a completion cell written later). Each function does exactly
   what its name says. */
#include <stdint.h>
#include <string.h>

static int64_t *kept_cell;
static unsigned char *kept_bytes;

/* Keeps the address of the cell and writes 1 through it now. */
static inline void cell_keep(int64_t *out) { kept_cell = out; *out = 1; }
/* Reads, or writes, through the address kept by `cell_keep`. */
static inline int64_t cell_read_later(void) { return *kept_cell; }
static inline void cell_write_later(int64_t v) { *kept_cell = v; }
/* Keeps the address of a buffer and fills it now, 32 bytes, or one. */
static inline int bytes_keep32(unsigned char *md) { kept_bytes = md; memset(md, 7, 32); return 1; }
static inline int bytes_keep(unsigned char *md) { kept_bytes = md; md[0] = 7; return 1; }
/* Writes through the address kept by either. */
static inline int64_t bytes_write_later(void) { kept_bytes[0] = 99; return 99; }
/* Writes `v` through the cell it is lent, and keeps nothing. */
static inline void put_one(int64_t *x, int64_t v) { *x = v; }

#endif
