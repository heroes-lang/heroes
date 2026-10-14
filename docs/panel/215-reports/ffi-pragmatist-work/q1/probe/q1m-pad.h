/* ffi-pragmatist, panel 215: a padded record and four C readers of its bytes. */
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
struct cd { char c; double d; };      /* 7 bytes of padding after c */
/* the byte readers, each taking the record's address as C's own APIs take a buffer */
static inline long w_cd(const struct cd *p) { return (long)write(1, p, sizeof *p); }
static inline int same_cd(const struct cd *a, const struct cd *b) { return memcmp(a, b, sizeof *a) == 0; }
static inline unsigned fnv_cd(const struct cd *p) {
    const unsigned char *b = (const unsigned char *)p; unsigned h = 2166136261u;
    for (size_t i = 0; i < sizeof *p; i++) { h ^= b[i]; h *= 16777619u; }
    return h & 1u; }
static inline long send_cd(int fd, const struct cd *p) { return (long)send(fd, p, sizeof *p, 0); }
