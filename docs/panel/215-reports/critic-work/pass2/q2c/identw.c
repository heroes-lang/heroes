/* Critic, panel 215 pass 2: the same type-identity question on Windows. */
#include <windows.h>
#include <stdint.h>
struct csbox { CRITICAL_SECTION cs; int32_t n; };
struct srwbox { SRWLOCK l; int32_t n; };
#define IS(T, M, U) __builtin_types_compatible_p(__typeof__(((T *)0)->M), U)
_Static_assert(IS(struct csbox, cs, CRITICAL_SECTION), "cs is a CRITICAL_SECTION");
_Static_assert(IS(struct srwbox, l, SRWLOCK), "l is an SRWLOCK");
_Static_assert(!IS(struct srwbox, l, CRITICAL_SECTION), "an SRWLOCK is not a CRITICAL_SECTION");
_Static_assert(!IS(struct csbox, n, CRITICAL_SECTION), "n is not");
int main(void) { return 0; }
