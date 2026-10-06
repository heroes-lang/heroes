/* Critic, panel 178: small padded structs, the size the ABI passes in registers. */
#include <unistd.h>
struct cd { char c; double d; };      /* 7 bytes of padding, 16 bytes */
struct ci { char c; int i; };         /* 3 bytes of padding, 8 bytes  */
struct fc { float f; char c; };       /* 3 bytes of TRAILING padding  */
static inline long w_cd(const struct cd *p) { return (long)write(1, p, sizeof *p); }
static inline long w_ci(const struct ci *p) { return (long)write(1, p, sizeof *p); }
static inline long w_fc(const struct fc *p) { return (long)write(1, p, sizeof *p); }
