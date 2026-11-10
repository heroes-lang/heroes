/* The program's own header in a folder of its own (defect 2420). */
#include <sys/timeb.h>
#define CLOCK_TICKS ((long long)TIMEB_OWN * 2)
