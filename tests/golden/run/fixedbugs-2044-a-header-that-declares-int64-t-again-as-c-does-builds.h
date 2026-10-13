/* Defect 2044, 2026-10-13: a header that declares `int64_t` again as the
 * type C's <stdint.h> declares, which C11 6.7p3 allows. */
typedef __INT64_TYPE__ int64_t;
static inline int64_t nine_billion(void) { return (int64_t)3000000000 * 3; }
