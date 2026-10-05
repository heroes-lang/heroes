/* Defect 245's third door, a binding's own C, through the runtime's copying
 * constructor: `hero_str_from_bytes` takes the bytes with their length, so a
 * NUL among them is kept, and the block it copies into is marked. The static
 * constructor is `fixedbugs-245-a-str-c-lays-out-holding-a-nul-stops.h`. */
static inline HeroStr made_by_c(void) { return hero_str_from_bytes("a\0b", 3); }
