/* Includes `fixedbugs-2429-sub.h`, which an earlier header has read under
 * its switch, and declares one name of its own (defect 2429). */
#include "fixedbugs-2429-sub.h"
static inline long long top_one(void) { return 2; }
