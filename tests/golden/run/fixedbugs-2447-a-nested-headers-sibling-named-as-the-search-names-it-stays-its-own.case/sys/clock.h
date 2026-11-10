/* The program's own header in a folder of its own, naming its sibling
   as the search names it, which C finds through the program's folder. */
#include "sys/timeb.h"
#define CLOCK_TICKS ((long long)TIMEB_OWN * 2)
