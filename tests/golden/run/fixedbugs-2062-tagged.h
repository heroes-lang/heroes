/* Defect 2062, 2026-10-14: a record holding a union, as panel 215's blind
 * readers were handed it. No include guard, as theirs had none. */
struct tagged { int kind; union { int fd; void *handle; } via; };
static inline int tg_kind(const struct tagged *t) { return t->kind; }
