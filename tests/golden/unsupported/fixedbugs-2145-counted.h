/* Defect 2145's header (panel 225's ffi-pragmatist): a constant clang
   refuses at its accessor, whose value expands __COUNTER__, before one whose
   value reads the count. */
#define K_BAD (__COUNTER__ + 1)
#define K_CNT2 ((-2147483647 - 1) / (__COUNTER__ - 3))
