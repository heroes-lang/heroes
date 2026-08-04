/* heroes_runtime.h — the C runtime interface (design.md §4.20).
 *
 * Generated C #includes this header, so clang type-checks every runtime call
 * against it. Keep declarations exact: this file IS the contract.
 *
 * M5b scope: `str` with reference counting, canonical `f64` rendering, and the
 * live-block counter. The rest of §4.20 (array, map, COW, join) arrives at M5c
 * with the representation spike 04 froze.
 *
 * HERO_RUNTIME_ABI is 2 because the declarations changed shape (CLAUDE.md §7):
 * every generated translation unit _Static_asserts it, so a `runtime/` from
 * another milestone is a compile error rather than a wrong answer.
 */

#ifndef HEROES_RUNTIME_H
#define HEROES_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HERO_RUNTIME_ABI 2

_Noreturn void hero_panic(const char *msg);
_Noreturn void hero_panic_overflow(void);
_Noreturn void hero_unreachable(void);

void hero_print_int(int64_t v);
void hero_print_bool(bool v);
void hero_print_end(void);

/* -- str (design.md §4.20) --------------------------------------------------
 * A str value is a fat pointer PASSED BY VALUE: the bytes and the refcount
 * live in one heap block, laid out as HeroStrHeader followed by len+1 bytes,
 * the last of which is NUL. So `.cstr()` is `s.ptr` — zero copies, §4.20's
 * "single highest-return decision".
 *
 * ptr == NULL is the ONE non-value: an unassigned or moved-out slot. Every
 * legitimate str has a non-NULL ptr, because "" is the static empty literal.
 * That is what lets the prologue zero-init a str slot (proposal point 2) while
 * keeping a read of an unassigned slot a LOUD panic instead of a silent "".
 */

typedef struct {
    const char *ptr; /* NUL-terminated; NULL only for the non-value */
    int64_t len;     /* bytes, excluding the NUL */
} HeroStr;

/* The heap block header, immediately before ptr. Declared (not private) so the
 * emitter's static literals can be laid out by clang rather than by malloc.
 * refcount < 0 means "static, never freed". */
typedef struct {
    int64_t refcount;
    uint64_t magic; /* HERO_STR_MAGIC — see hero_str_decref */
} HeroStrHeader;

/* The tag exists because HeroStr is a plain two-field struct, so
 * `HeroStr fake = {c_pointer, n};` compiles with zero warnings even under
 * -Weverything, and decref then decrements a word inside a BOUND LIBRARY's heap
 * block. Measured (panel 021): with SQLite's own text buffer that write landed
 * inside a valid allocation, so ASan said nothing and the program exited 0 —
 * silent corruption of foreign memory. The tag turns it into a panic. */
#define HERO_STR_MAGIC UINT64_C(0x4845524f53535452) /* "HEROSSTR" */

/* A literal is a static const block: no allocation, no runtime call, and a
 * negative refcount so decref is a no-op. The emitter writes one of these per
 * distinct literal at file scope and then `s = HERO_STR_LIT(name);`. */
#define HERO_STR_STATIC(name, text)                     \
    static const struct {                               \
        HeroStrHeader h;                                \
        char b[sizeof(text)];                           \
    } name = {{-1, HERO_STR_MAGIC}, text}
#define HERO_STR_LIT(name) \
    ((HeroStr){(name).b, (int64_t)sizeof((name).b) - 1})

HeroStr hero_str_empty(void); /* "" — non-NULL ptr, static block */

void hero_str_incref(HeroStr s);
void hero_str_decref(HeroStr s); /* no-op on the NULL non-value */

HeroStr hero_str_concat(HeroStr a, HeroStr b);
bool hero_str_eq(HeroStr a, HeroStr b);
int64_t hero_str_cmp(HeroStr a, HeroStr b); /* <0, 0, >0 — the six comparisons */
int64_t hero_str_len(HeroStr s);
int64_t hero_str_byte(HeroStr s, int64_t i);        /* aborts out of range */
HeroStr hero_str_slice(HeroStr s, int64_t from, int64_t to);
void hero_print_str(HeroStr s);

/* cstr: the FFI spelling (design.md §4.19). Free because of the NUL. */
const char *hero_str_cstr(HeroStr s);

/* THE MISSING PRIMITIVE (panel 021). §4.19's ladder step 3 is "open a database,
 * run a query, READ A RESULT, close". sqlite3_column_text hands back a borrowed
 * pointer SQLite invalidates on the next step(); every C library that returns a
 * string does the same. Landing one in a Heroes str slot requires an owning
 * COPY, and the emitter must never fabricate a HeroStr from a raw C pointer:
 * decref would compute a header from foreign memory. */
HeroStr hero_str_from_bytes(const char *p, int64_t len);
HeroStr hero_str_from_cstr(const char *p); /* strlen, then from_bytes */

/* -- f64 (proposal point 4) ------------------------------------------------ */
void hero_print_f64(double v);
HeroStr hero_f64_to_str(double v);
HeroStr hero_int_to_str(int64_t v);
HeroStr hero_bool_to_str(bool v);
HeroStr hero_str_identity(HeroStr s);

/* -- the leak check ASan cannot do on this platform ------------------------
 * MEASURED (panel 021): AddressSanitizer on Darwin arm64 has NO
 * LeakSanitizer. `ASAN_OPTIONS=detect_leaks=1` aborts with "not supported on
 * this platform" and a 1234-byte unfreed malloc exits 0 in silence. So the
 * runtime counts its own live blocks and the golden harness asserts the
 * balance at exit: portable, deterministic, and it sees a missing decref.
 * hero_runtime_live() is the count; hero_runtime_check_leaks() panics if
 * non-zero and is what the emitted main() calls under --check-leaks. */
int64_t hero_runtime_live(void);
void hero_runtime_check_leaks(void);

#endif /* HEROES_RUNTIME_H */
