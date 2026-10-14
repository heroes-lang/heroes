/* Critic, panel 215 pass 2: uni.h's anonymous union, with a declared field
   AFTER it, so the bytes of `h` that `fd` does not cover lie BETWEEN two
   declared fields rather than at the struct's end. */
#include <stdint.h>
struct m { int32_t kind; union { int32_t fd; void *h; }; int32_t tail; };
static inline void set_h(struct m *p) { p->h = (void *)(uintptr_t)0x1122334455667788ull; }
static inline int64_t get_h(const struct m *p) { return (int64_t)(uintptr_t)p->h; }
