/* The header of a group whose program binds a macro over `int64_t`, a type
 * of C's own library the unit's C spells by its name, defined after its own
 * code as a C header may not (C11 7.1.3). */
static inline long long told_seven(void) { return 7; }

#define int64_t 5
