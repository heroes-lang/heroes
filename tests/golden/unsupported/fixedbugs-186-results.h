/* Beside defect 186's cases, lane ffimsg, 2026-10-03: results and
   constants whose types the cases declare otherwise, each of a type whose
   spelling is the same on every platform. */
#include <stdint.h>

typedef struct opaque opaque_t;

static inline int64_t wide_count(void) { return 5000000000LL; }
static inline void reset(void) { }
static inline int status(void) { return 0; }
static inline double ratio(void) { return 0.5; }
static inline opaque_t *open_one(void) { return 0; }

#define BIG_COUNT 5000000000LL
#define HALF 0.5
