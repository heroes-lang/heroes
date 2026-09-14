/* Two handle types, and one call that hands BOTH back through out-parameters.
 * The shape defect 034 inverted: the emitter wrote one `hero_handle_acquired();`
 * per CALL rather than per MARK, so the program that correctly released both
 * aborted at 134 and the one that released one exited 0. */
#include <stdint.h>

typedef struct Slot Slot;
typedef struct Conn Conn;

struct Slot { int64_t n; };
struct Conn { int64_t m; };

static inline Slot *slot_open(int64_t n) {
    static struct Slot pool[8];
    static int64_t i = 0;
    pool[i].n = n;
    return &pool[i++];
}

static inline Conn *conn_open(int64_t n) {
    static struct Conn pool[8];
    static int64_t i = 0;
    pool[i].m = n;
    return &pool[i++];
}

static inline void slot_close(Slot *s) { (void)s; }
static inline void conn_close(Conn *c) { (void)c; }

static inline void both_open(int64_t n, Slot **a, Conn **b) {
    *a = slot_open(n);
    *b = conn_open(n + 1);
}
