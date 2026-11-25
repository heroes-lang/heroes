/* The header of a group that defines, as a C header may, a macro over every
 * word the unit's own C wrote bare until 2026-11-24, after its own code.
 * Each was an internal error at exit 2 or a refusal told as this header's:
 * a `T?`'s and a variant's members, a thread's box, a case's member, a
 * block's label, a probe's parameters, a renderer's, a map lookup's local,
 * the attributes the unit spells, C's keywords, and the words of the pragmas
 * that give a header's state back after the groups.
 *
 * It reads the C library's two headers the unit reads after its groups
 * (`runtime/heroes_standard.h`) before it defines a word, as a header that
 * defines such words must in any C program: a word the library's own headers
 * use breaks them when they are read after it, a true `ffi_header_refused`.
 * Measured 2026-11-25: `#define a1 1` broke mingw-w64's `math.h:164`, where
 * `a1` is a parameter (zig 0.15.2's headers), and the CI's MSVC `<stdint.h>`
 * read after these words left its `#pragma pack` changed (run of fc19cb341).
 * Read here first, each is read once, before any word below. */
#include <stdint.h>
#include <math.h>

struct macro_pair { long long left; long long right; };

#define MACRO_LIMIT 99

static inline long long macro_seven(void) { return 7; }
static inline long long macro_add(long long x, long long y) { return x + y; }
static inline long long macro_apply(long long (*callback)(long long), long long x) { return callback(x); }
static inline long long macro_sum(struct macro_pair pair) { return pair.left + pair.right; }
static inline void macro_grow(struct macro_pair *pair) { pair->left += 1; }

/* The members of a `T?`, a variant and a thread's box, and a case's member. */
#define tag 1
#define as 1
#define ok 1
#define err 1
#define head 1
#define c_circle 1
#define c_rect 1
/* A block's label, a probe's parameters, a renderer's, a lookup's local. */
#define bb0 1
#define bb1 1
#define bb2 1
#define bb3 1
#define a0 1
#define a1 1
#define o 1
#define found 1
#define which 1
/* The attributes the unit spelled, and a keyword it writes. */
#define unused 1
#define used 1
#define nodebug 1
#define constructor 1
#define no_sanitize 1
#define default 1
/* Every word of the pragmas that give a header's state back. */
#define pop 1
#define push 1
#define on 1
#define off 1
#define precise 1
#define bss 1
#define data 1
#define rodata 1
#define relro 1
#define section 1
#define optimize 1
#define visibility 1
#define pack 1
#define float_control 1
#define diagnostic 1
#define ignored 1
#define error 1
