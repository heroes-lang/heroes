/* The header of a group whose program binds, as constants, a macro over
 * names C's library defines or declares after the groups, `<stdint.h>`'s and
 * `<math.h>`'s, a macro reaching two of them, and a record whose fields are
 * named as `<math.h>` and `<stdint.h>` name their macros. */
static inline long long after_seven(void) { return 7; }

struct after_word { long long M_E; long long UINT16_MAX; long long HUGE_VALL; long long FP_ZERO; };

#define INT64_MAX 1
#define INT32_MAX 2
#define UINT8_MAX 3
#define SIZE_MAX 4
#define INTMAX_MAX 5
#define NAN 6
#define INFINITY 7
#define HUGE_VALF 8
#define FP_NAN 9
#define M_PI 10
#define isnan 11
#define y1 12
#define j0 13
#define OVERFLOW 14
#define INT16_MAX 15
#define INT8_MIN 16
#define WIDEST INT16_MAX
#define TINIEST INT8_MIN
