/* Defect 2386: group records whose `const` member lies inside another of
 * their members, where clang refuses every assignment of the record in its
 * words for a NESTED const member; each also handed back by value. */
#include <stdint.h>

struct fixedbugs_2386_in { const unsigned k : 3; unsigned b : 2; };

/* A `const` bit-field in a named record member. */
typedef struct fixedbugs_2386_nested { int32_t a; struct fixedbugs_2386_in in; double d; } fixedbugs_2386_nested;

/* The same in an anonymous struct member. */
typedef struct fixedbugs_2386_anon { int32_t a; struct { const unsigned k : 3; unsigned b : 2; }; double d; } fixedbugs_2386_anon;

/* The same in an arm of an anonymous union. */
typedef struct fixedbugs_2386_arm { int32_t a; union { const unsigned k : 3; unsigned char c; }; double d; } fixedbugs_2386_arm;

/* A `const` member, no bit-field, two records deep. */
struct fixedbugs_2386_leaf { const int32_t k; int32_t b; };
typedef struct fixedbugs_2386_mid { int32_t m; struct fixedbugs_2386_leaf leaf; } fixedbugs_2386_mid;
typedef struct fixedbugs_2386_deep { int32_t a; fixedbugs_2386_mid mid; double d; } fixedbugs_2386_deep;

static inline fixedbugs_2386_nested fixedbugs_2386_nested_make(void) { fixedbugs_2386_nested v = { 1, { 2, 1 }, 3.0 }; return v; }
static inline fixedbugs_2386_anon fixedbugs_2386_anon_make(void) { fixedbugs_2386_anon v = { 1, { 2, 1 }, 3.0 }; return v; }
static inline fixedbugs_2386_arm fixedbugs_2386_arm_make(void) { fixedbugs_2386_arm v = { 1, { 2 }, 3.0 }; return v; }
static inline fixedbugs_2386_deep fixedbugs_2386_deep_make(void) { fixedbugs_2386_deep v = { 1, { 2, { 3, 4 } }, 5.0 }; return v; }
