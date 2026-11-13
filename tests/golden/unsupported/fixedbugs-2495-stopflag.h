/* Defect 2495, lane b34-fsl, 2026-11-13: an object declared here, defined by
   no header any module names, written and read by two inline bodies. */
#include <stdint.h>
extern volatile int stop2495_requested;
static inline void stop2495_request(void) { stop2495_requested = 1; }
static inline int64_t stop2495_asked(void) { return stop2495_requested; }
