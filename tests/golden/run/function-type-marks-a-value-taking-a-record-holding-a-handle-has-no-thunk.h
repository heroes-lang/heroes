/* Lane land261: a handle a record of the program holds, and a C function
 * taking a callback, so the program's unit carries the thunks' table. */
#ifndef RECORD_HOLDING_A_HANDLE_H
#define RECORD_HOLDING_A_HANDLE_H
#include <stdint.h>
typedef struct box box;
struct box { int64_t v; };
static box the_box = {7};
static inline box *box_get(void) { return &the_box; }
static inline int64_t box_value(box *b) { return b->v; }
static inline int64_t call_twice(int64_t (*cb)(int64_t)) { return cb(1) + cb(2); }
#endif
