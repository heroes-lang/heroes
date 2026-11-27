/* Defect 2876: a library of the program's own whose functions are named as
 * members of the Heroes runtime's structs are, none of them a name the
 * runtime's header declares. */
static inline long long hero_code(void) { return 41; }
static inline long long hero_header(void) { return 5; }
