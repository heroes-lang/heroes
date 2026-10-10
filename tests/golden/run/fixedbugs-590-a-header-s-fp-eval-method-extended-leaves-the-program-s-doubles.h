/* Defect 590, 2026-10-11: the extended evaluation method, which in the
   Linux arm64 image evaluates a double expression in a 128-bit long double. */
#pragma clang fp eval_method(extended)
#include <stdlib.h>
