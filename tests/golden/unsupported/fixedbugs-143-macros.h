/* Beside the cases of defect 143 and panel 185's R1, lane ffi-macro,
   2026-10-02: a header of the program's own, so no platform lacks it, with a
   function-like macro, an object-like one, a function that is also a macro,
   and the function of the program's own that reaches the first. */
#include <stdint.h>

/* a function-like macro, and nothing else of that name */
#define SQUARE(x) ((x) * (x))

/* an object-like macro over a number */
#define LIMIT 100

/* a function that is also a macro of the same name, as `getc` is on some
   libcs and `curl_easy_setopt` is in curl.h */
static inline int32_t twice(int32_t x) { return x * 2; }
#define twice(x) twice(x)

/* the repair the message names: a function of the program's own */
static inline int32_t hero_SQUARE(int32_t x) { return SQUARE(x); }
