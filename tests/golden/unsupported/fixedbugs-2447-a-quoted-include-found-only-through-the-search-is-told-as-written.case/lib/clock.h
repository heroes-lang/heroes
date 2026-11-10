/* The program's own header, naming <sys/timeb.h> in quotes as the search
   names it, which C does not find in lib/. */
#include "sys/timeb.h"
#define CLOCK_TICKS ((long long)TIMEB_OWN * 2)
