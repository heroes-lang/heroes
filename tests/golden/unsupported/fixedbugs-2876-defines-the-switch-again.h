/* Defect 2876: a header of the program's own that takes back the switch the
 * Heroes runtime's `heroes_unit.h` defines, and defines it again. */
#undef HERO_STANDARD_AFTER_GROUPS
#define HERO_STANDARD_AFTER_GROUPS 7
