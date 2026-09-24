/* The correct out-parameter for a handle: the header spells it `T **` and
 * writes a pointer INTO the cell — `sqlite3_open`'s shape. Defect 083's
 * control: the depth question must accept this, on a `void **` and on a
 * typedef that hides the pointer, since clang sees through it. */
#include <stdlib.h>
#include <stdint.h>
typedef void *handle_t;
static inline __attribute__((noinline)) int32_t make_into(void **out) { *out = malloc(8); return *out == NULL ? 1 : 0; }
static inline __attribute__((noinline)) int32_t make_typedef(handle_t *out) { *out = malloc(8); return *out == NULL ? 1 : 0; }
static inline __attribute__((noinline)) void eat(void *p) { free(p); }
