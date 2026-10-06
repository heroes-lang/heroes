/* Included by `fixedbugs-361-a-macro-from-a-header-the-group-includes-is-
 * kept-off-too.h`, so the program binds nothing of it. */
#include <stdbool.h>
#define hero_print_int nothing
#undef bool
#define bool int
#undef true
#define true 0
#define HeroStr int
