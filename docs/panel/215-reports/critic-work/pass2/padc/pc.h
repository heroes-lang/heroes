/* Critic, panel 215 pass 2: the shapes beside route G's tested ones. Each
   writer hands every byte of what it is lent to write(2) on /dev/null, which
   MemorySanitizer's interceptor checks byte for byte. */
#ifndef PC_H
#define PC_H
#include <fcntl.h>
#include <unistd.h>
struct cd { char c; double d; };   /* 7 bytes of padding inside, 16 bytes */
struct tp { double d; char c; };   /* 7 bytes of TRAILING padding, 16 bytes */
struct ocd { struct cd inner; };   /* the padding inside a member, 16 bytes */
static inline int pc_null(void) { static int fd = -1; if (fd < 0) fd = open("/dev/null", O_WRONLY); return fd; }
static inline long w_cd(const struct cd *p) { return (long)write(pc_null(), p, sizeof *p); }
static inline long w_tp(const struct tp *p) { return (long)write(pc_null(), p, sizeof *p); }
static inline long w_ocd(const struct ocd *p) { return (long)write(pc_null(), p, sizeof *p); }
static inline long w_cds(const struct cd *p) { return (long)write(pc_null(), p, 2 * sizeof *p); }
#endif
