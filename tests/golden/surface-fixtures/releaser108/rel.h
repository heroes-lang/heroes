/* Handles for releaser108/: one struct, three releasers, and a call of each
   shape the fixture's marks ride. */
#include <stdint.h>
typedef struct hh hh;
struct hh { int64_t v; };
static hh pool[8];
static int64_t next_one = 0;
static inline int64_t h_open(int64_t n, hh **out) { pool[next_one].v = n; *out = &pool[next_one++]; return 0; }
static inline int64_t h_make(int64_t n, hh **out) { pool[next_one].v = n; *out = &pool[next_one++]; return 0; }
static inline void h_keep(hh *into, hh *x) { into->v = x->v; }
static inline void h_ref(hh *x) { (void)x; }
static inline void h_close(hh *x) { x->v = -1; }
static inline void h_close_v2(hh *x) { x->v = -2; }
static inline void h_close_v3(hh *x) { x->v = -3; }
static inline int64_t h_value(hh *x) { return x->v; }
