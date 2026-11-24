#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct Slot Slot;
struct Slot { int64_t n; };
static inline Slot *slot_open(int64_t n) { Slot *s = malloc(sizeof *s); s->n = n; return s; }
static inline void slot_close(Slot *s) { free(s); }
static inline int64_t slot_value(Slot *s) { return s->n; }
typedef int64_t (*hj_cb_t)(int64_t);
static jmp_buf hj_env;
static inline void hj_throw(void) { longjmp(hj_env, 1); }
static inline int64_t hj_try(hj_cb_t cb) { if (setjmp(hj_env) == 0) { cb(1); return -1; } return 7; }
