/* Defect 2062, 2026-10-14: the header of the program's own the note's third
 * spelling describes, which includes the group's and reaches the union's
 * members by macros, as `fixedbugs-151-unions.h` reaches `s6_addr`. */
#include "fixedbugs-2062-tagged.h"
#define fd via.fd
#define handle via.handle
