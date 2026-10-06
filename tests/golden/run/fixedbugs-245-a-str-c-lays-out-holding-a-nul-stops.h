/* Defect 245's third door, a binding's own C, through the runtime's static
 * constructor: `HERO_STR_STATIC` lays a literal out at C compile time, so the
 * mark saying it holds a NUL is computed there, by clang, from the literal's
 * size against its length to the first NUL. The copying constructor is
 * `fixedbugs-245-a-str-c-copies-holding-a-nul-stops.h`. */
HERO_STR_STATIC(fixedbugs_245_static, "a\0bc");
static inline HeroStr laid_out_by_c(void) { return HERO_STR_LIT(fixedbugs_245_static); }
