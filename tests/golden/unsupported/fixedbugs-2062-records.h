/* Defect 2062, 2026-10-14: records whose fields a program may name only
 * inside another member, the shape panel 215's blind readers met first. */
struct tagged { int kind; union { int fd; void *handle; } via; };
struct nested { int kind; struct { int x; int y; } pos; };
struct deep { int kind; struct { union { int fd; void *h; } u; } inner; };
union named_u { int fd; void *handle; };
struct holds { int kind; union named_u via; };
static inline int tg_kind(const struct tagged *t) { return t->kind; }
