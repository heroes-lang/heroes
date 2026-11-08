/* The package `pk`'s own header: constants clang's front end can read, and
 * constants only a run of the package's code could judge (defect 2351). */
#include <stdint.h>

#define PK_HALF 0.5
enum pk_colour { PK_RED = 7, PK_GREEN };
#define PK_MINDIV (INT64_MIN / -1)
static const int pk_k = 5;
#define PK_OBJECT pk_k
#define PK_FINE ((1LL << 40) + 3)
