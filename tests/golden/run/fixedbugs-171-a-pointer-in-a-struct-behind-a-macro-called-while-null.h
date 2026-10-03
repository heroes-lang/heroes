/* Beside defect 171's run cases, lane ffimsg, 2026-10-03: a function
   pointer held in a struct, reached by a macro under the API's name. */
#include <stdint.h>

struct ops { int32_t (*f)(int32_t); };
static struct ops the_ops = { 0 };
#define ops_f the_ops.f
