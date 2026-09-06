/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* heroes_runtime.h — the C runtime interface (design.md §4.20).
 *
 * Generated C #includes this header, so clang type-checks every runtime call
 * against it. Keep declarations exact: this file IS the contract.
 *
 * M-strings-ownership scope: `str` with reference counting, canonical `f64` rendering, and the
 * live-block counter. M-value-aggregates adds the descriptor ABI and `[T]`, with the
 * representation spike 04 froze — the map and COW's write-back follow.
 *
 * HERO_RUNTIME_ABI moves whenever the declarations change shape (CLAUDE.md §7):
 * every generated translation unit _Static_asserts it, so a `runtime/` from
 * another milestone is a compile error rather than a wrong answer. **The number
 * itself is below and nowhere else** — this line said "is 3" while the `#define`
 * said 14, for eleven milestones, and a reader who trusted the prose was reading
 * a count that had expired in silence (found by panel 088's ffi-pragmatist,
 * 2026-08-23; CLAUDE.md §11's named failure). What the stamp catches is version
 * skew and not a decoy: a `runtime/` that copies the number passes, and the cache
 * key is what covers a decoy's contents.
 */

#ifndef HEROES_RUNTIME_H
#define HEROES_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HERO_RUNTIME_ABI 21

_Noreturn void hero_panic(const char *msg);
_Noreturn void hero_panic_overflow(void);
_Noreturn void hero_unreachable(void);

/* The entry of a function this program hands to C as a callback, so that C
 * calling it back from a thread of its own stops by name instead of corrupting
 * a refcount (panel 111 R9; parts/thread.c carries the reasoning and the
 * measurements). `what` is the Heroes name, `module.function`, because the
 * reader of the message is looking at a .hero file. It is DECLARED HERE, under
 * the ABI stamp, on purpose: a runtime that predates the guard must be a
 * compile error rather than a program that quietly runs unguarded. */
void hero_thread_guard(const char *what);

/* `heroes test`'s argument, parsed here so a generated unit still includes this
 * header and nothing else (CLAUDE.md §7). */
int64_t hero_test_index(int argc, char **argv);

void hero_print_int(int64_t v);
void hero_print_uint(uint64_t v);
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

/* -- the reference count, and why it has a name (design.md:2618-2624) --------
 *
 * design.md names two v1 invariants and this is the second one: keep refcount
 * operations behind a narrow, never-inlined boundary *"so no inlining can smear
 * refcount arithmetic across code that a thread-local counter would later have
 * to change"*. The boundary was built and held. The counter behind it stayed a
 * plain `int64_t` in three structs until M-isolated-threads step 3, and panel
 * 111 measured what that costs the day C hands a Heroes function to a thread of
 * its own: a shared `str` across 32 threads is `heap-use-after-free` or a double
 * free in 9 ASan runs of 10, and without a sanitizer the count drifts to
 * 6290-9785 where it should be 1.
 *
 * ONE TYPEDEF RATHER THAN THREE FIELDS, because the invariant is one: `str`,
 * `[T]` and `{K: V}` are counted the same way, and three independent `_Atomic
 * int64_t`s would be three places to forget. `_Atomic` and not a discipline of
 * calling the right builtin on a plain integer: a plain field lets an ordinary
 * `+= 1` compile and be wrong, where this one makes the same line correct (and
 * sequentially consistent, which is slower and never wrong). The two hot paths
 * ask for a weaker order explicitly, in `str.c`, `array.c` and `map.c`, and
 * nowhere else.
 *
 * WHAT THIS DOES NOT FIX, named here because the cheap half is the easy half:
 * `cow.c`'s `if (refcount == 1)` is a test and then a mutate, so two threads can
 * both read 1 and both mutate. An atomic load makes that read whole; it does not
 * make the pair one operation. That is a protocol, it is this milestone's, and
 * `cow.c` carries the sentence at the line itself. */
typedef _Atomic int64_t HeroRefcount;

/* THE LAYOUT MUST NOT MOVE, and it is asserted rather than assumed. The emitter
 * lays out a static literal's header with `HERO_STR_STATIC` below, so a wider or
 * differently aligned counter would silently change what generated C emits.
 * Lock-free matters for the same literal from the other side: its block is
 * `static const`, so it can live in read-only memory, and a load that a lock had
 * to guard would take a write lock on a page nobody may write.
 *
 * The lock-free question is asked with `__atomic_always_lock_free` rather than
 * with `<stdatomic.h>`'s `ATOMIC_LLONG_LOCK_FREE`, for two reasons that are both
 * facts about this repository: that macro names `long long`, and `int64_t` is
 * `long` on one of the three platforms here, so it would answer about a type
 * this header does not use; and the builtin needs no `#include`, which keeps
 * this file — the one every generated translation unit reads — at the three
 * includes it already had. */
_Static_assert(sizeof(HeroRefcount) == sizeof(int64_t), "the refcount changed width");
_Static_assert(_Alignof(HeroRefcount) == _Alignof(int64_t), "the refcount changed alignment");
_Static_assert(__atomic_always_lock_free(sizeof(HeroRefcount), 0),
               "a 64-bit atomic needs a lock here, and a str literal's block is read-only");

/* The heap block header, immediately before ptr. Declared (not private) so the
 * emitter's static literals can be laid out by clang rather than by malloc.
 * refcount < 0 means "static, never freed". */
typedef struct {
    HeroRefcount refcount;
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

/* Aborts out of range, AND aborts on a cut that lands inside a multi-byte
 * sequence — design.md:809's "slicing that lands mid-sequence is an error",
 * which this runtime did not implement until panel 027 went looking for it.
 * It is the only operation in the language that can break a `str`'s
 * well-formedness, which is why the rule lives here rather than at `chars`. */
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
/* Is this byte range well-formed UTF-8?
 *
 * **Exported so a binding can branch instead of dying.** `hero_str_from_bytes`
 * aborts on ill-formed input, which is right for the *program's* own bytes and
 * wrong for the *environment's*: a Latin-1 `PATH`, a PNG, a SQLite TEXT column
 * another program wrote. Without this, a binding author's only alternatives were
 * to let it abort or to copy the validator — two validators that can silently
 * disagree (ffi-pragmatist, panel 035 item 4). One entry point, single-sourced.
 *
 * Adding a *function* to this ABI is self-guarding: an old runtime is an
 * undefined symbol at link. Adding a struct field is not, which is panel 027's
 * own asymmetry. */
bool hero_utf8_valid(const char *p, int64_t len);

HeroStr hero_str_from_bytes(const char *p, int64_t len);
HeroStr hero_str_repeat(HeroStr s, uint64_t n); /* n copies, one allocation */
HeroStr hero_str_from_cstr(const char *p); /* strlen, then from_bytes */

/* The same conversion with a status instead of a grave (panel 089). The Heroes
 * side is `validated(c: cstr) -> str?` in the Tier-2 library, and these three
 * codes are what it branches on — small integers rather than errno, for the
 * reason `hero_os.h` gives about its own set: the `e.code` string a program sees
 * must be the same on every platform.
 *
 * **The codes are written HERE and nowhere else.** M-header-constants moved
 * `hero_os.h`'s set into its header for exactly this, and `library_source.hero`
 * records the rule in its own words: the header is the only place they live, so
 * renumbering cannot leave the Heroes side behind. */
#define HERO_STR_OK 0
#define HERO_STR_NULL 1
#define HERO_STR_NOT_TEXT 2
HeroStr hero_str_try_from_cstr(const char *p, int64_t *status);
/* The same guard on the outbound side: a `cstr` handed to a C function, which is
   the path `hero_str_from_cstr` never sees (panel 053, CLAUDE.md §12). */
const char *hero_cstr_nonnull(const char *p);

/* -- f64 (proposal point 4) ------------------------------------------------ */
float hero_int_to_f32(int64_t v);
float hero_f64_to_f32(double v);
double hero_f32_to_f64(float v);
void hero_print_f32(float v);
HeroStr hero_f32_to_str(float v);
void hero_print_f64(double v);
HeroStr hero_f64_to_str(double v);
HeroStr hero_int_to_str(int64_t v);
HeroStr hero_uint_to_str(uint64_t v);
HeroStr hero_bool_to_str(bool v);
HeroStr hero_str_identity(HeroStr s);

/* `to_i64` truncates toward zero; out of range or NaN ABORTS, because
 * `(int64_t)v` past int64's range is C11 6.3.1.4p1 undefined behaviour and
 * arm64's `fcvtzs` saturates rather than trapping. `to_f64` cannot fail, and is
 * lossy above 2^53 — defined, silent, and Part 8's to record. */
int64_t hero_f64_to_int(double v);
bool hero_f64_fits_int(double v);
double hero_int_to_f64(int64_t v);

/* -- the failure side of `T?` (design.md §4.6) -------------------------------
 *
 * `fail(code, msg)`, and the two fields the spec names: a stable snake_case code
 * and a human message. Declared here rather than generated because it is the one
 * record every `T?` in every program contains, and because §4.6 fixes its shape —
 * there is no declaration in the source for the compiler to generate it from.
 *
 * Two `HeroStr` is 32 bytes, which is what makes a `T?` 40 bytes while `T` fits in
 * 32 (measured; panel 023 corrected the earlier "40 for every T" — `sizeof(Big?)`
 * is 48 for a 40-byte record, because past 32 the payload wins). */

typedef struct HeroFailure {
    HeroStr code;
    HeroStr msg;
} HeroFailure;

void hero_failure_retain(const HeroFailure *v);
void hero_failure_release(HeroFailure *v);
bool hero_failure_eq(const HeroFailure *a, const HeroFailure *b);
uint64_t hero_failure_hash(const void *elem);

/* The failure `m[k]` produces when the key is absent (spec line 125-126: code
 * `missing_key`). Both strings are static blocks, so a lookup that misses
 * allocates nothing — which matters because a miss is the common case in a
 * `.default(v)` chain. */
HeroFailure hero_failure_missing_key(void);
HeroFailure hero_failure_does_not_fit(void);

/* `.must()` on an error (§4.6). Takes the failure, because the useful half of the
 * message is the `code` and `msg` the author wrote — a panic saying only that a
 * `.must()` failed tells the reader the one thing they already know. */
_Noreturn void hero_panic_must(HeroFailure f);

/* `assert` (§4.18). Two entry points because C has no optional arguments and the
 * spec asks for both the source expression AND both sides — the second is what a
 * bare "assert failed" loses. The emitter renders each side through the same
 * `to_str` entry points `print` uses, and falls back to the text-only form where
 * a side has no rendering (a record, an array). */
_Noreturn void hero_panic_assert(HeroStr text);
_Noreturn void hero_panic_assert_sides(HeroStr text, HeroStr left, HeroStr right);

/* -- the descriptor ABI (design.md §4.20, panels 021, 022) -------------------
 *
 * C has no copy constructors, no destructors and no generic comparison, while
 * §4.3 demands structural `==` recursively and §4.10 demands value-semantics
 * copies and drops. So the COMPILER generates them, one small set per reachable
 * type, and the runtime works through a descriptor. The alternative — a
 * type-erased void* runtime that inspects values itself — would void the
 * property this whole backend rests on, that clang type-checks every call.
 *
 * `copy` is SHALLOW plus incref, not deep (panel 022): copy-on-write is what
 * makes a deep copy unnecessary, because sharing is unobservable until somebody
 * mutates and the mutation primitives unshare. Deep copying here would pay for
 * every binding what only a mutation costs.
 *
 * `hash` is generated for EVERY type and is never null, even where no map uses
 * it. A call through a null one is `SEGV on unknown address 0x0, pc 0x0` — no
 * type name, no source line — and Go's cheaper rule (emit it only for map-key
 * types) reintroduces exactly that (panel 022).
 *
 * `eq` and `hash` walk FIELDS, never bytes. `record Flag { n: i64, on: bool }`
 * carries 7 padding bytes, so two `==`-equal records hash differently under
 * memcmp with no warning, no error and no sanitiser report. */

typedef struct HeroDesc HeroDesc;
struct HeroDesc {
    size_t size;                              /* one element, in bytes        */
    void (*copy)(void *dst, const void *src); /* shallow + incref            */
    void (*drop)(void *elem);                 /* release what it owns        */
    bool (*eq)(const void *a, const void *b); /* structural equality (§4.3)  */
    uint64_t (*hash)(const void *elem);       /* never null                  */
};

/* The scalars and `str`, so the compiler never writes a descriptor for a type it
 * did not declare. `str`'s copy increfs; a scalar's is a plain assignment. */
extern const HeroDesc hero_desc_int;
/* The other seven widths. Separate descriptors, never aliases of the one above:
 * each carries its own `size`, and `sort.c` picks a comparison by pointer
 * identity (M-sized-integers, panel 042). */
extern const HeroDesc hero_desc_i8;
extern const HeroDesc hero_desc_i16;
extern const HeroDesc hero_desc_i32;
extern const HeroDesc hero_desc_u8;
extern const HeroDesc hero_desc_u16;
extern const HeroDesc hero_desc_u32;
extern const HeroDesc hero_desc_u64;
extern const HeroDesc hero_desc_f32;
extern const HeroDesc hero_desc_f64;
extern const HeroDesc hero_desc_bool;
extern const HeroDesc hero_desc_str;

/* And ONE for every `[T]`, whatever T is: copy/drop/eq on an array value work
 * through the header's own `elem`, so the descriptor of an array needs to know
 * nothing about what the array holds. `[[i64]]` and `[[str]]` share this. */
extern const HeroDesc hero_desc_array;
extern const HeroDesc hero_desc_failure;

/* And ONE for every function type: a Heroes function value is a bare C function
 * pointer (no closures in v1), so copy is an assignment, drop is nothing, and
 * equality is pointer identity. Shared for the same reason the array's is. */
extern const HeroDesc hero_desc_func;

/* -- the array: `[T]` (design.md §4.20, spike 04) ---------------------------
 *
 * A pointer to a heap header followed by the elements IN-LINE. The array is
 * Heroes' only indirection (§4.10), which is what gives a recursive type a
 * finite size — and being one pointer wide is why an array field imposes no
 * ordering constraint on C.
 *
 * NULL is the one non-value, exactly as for `str`: a zero-initialised slot the
 * exit sweep can release unconditionally. Every entry point rejects it loudly
 * rather than treating it as empty. */

typedef struct HeroArrayHeader {
    HeroRefcount refcount;
    int64_t len;
    int64_t cap;
    const HeroDesc *elem;
    /* elements follow in-line: cap * elem->size bytes */
} HeroArrayHeader;

HeroArrayHeader *hero_array_new(const HeroDesc *elem, int64_t cap);
void hero_array_incref(HeroArrayHeader *a);
void hero_array_decref(HeroArrayHeader *a); /* no-op on NULL */
int64_t hero_array_len(const HeroArrayHeader *a);

/* Read one element. Aborts out of range (spec line 126) — never reads
 * arbitrary memory, which is the guarantee §4.9 states. */
const void *hero_array_at(const HeroArrayHeader *a, int64_t index);

/* `push(xs, v)` — a NEW array, always, and the copy is not an oversight.
 *
 * `xs = [1, 2, 3]` then `push(xs, 4)` leaves `xs` observable, and its slot holds
 * one reference — so a refcount of 1 means "only the slot has it", and appending
 * in place would change what that slot sees. Value semantics has no reading in
 * which the argument can be mutated. The bill is §4.10's declared one: building
 * an array by successive push is O(n^2), the same shape as `s + t`, and
 * performance is a non-goal (Part 2). */
HeroArrayHeader *hero_array_push(const HeroArrayHeader *a, const void *elem);

/* Structural equality, element by element through the descriptor. */
bool hero_array_eq(const HeroArrayHeader *a, const HeroArrayHeader *b);

/* `slice(xs, from:, to:)` — a NEW array, elements copied through the descriptor.
 * ABORTS out of range, with the same three-part test as `hero_str_slice`: a
 * clamping slice hands a shorter array to whatever comes next, and when that is
 * a length passed to C the mismatch is silent (panel 027 R3). */
HeroArrayHeader *hero_array_slice(const HeroArrayHeader *a, int64_t from, int64_t to);

/* `sort(xs)` — a NEW array, STABLE, ascending.
 *
 * Works on `[i64]`, `[f64]` and `[str]`, which is exactly the set with an order;
 * the comparison lives inside `runtime.c` and is NOT part of this contract,
 * because panel 027 vetoed putting a `cmp` in `HeroDesc`: C11 6.7.9p21
 * zero-fills a short initialiser list, so every descriptor that forgot the field
 * would carry a NULL and SEGV with no type name — panel 022's null-`hash`
 * argument, one field over. A `sort` on any other element type is refused by the
 * emitter's gate, so reaching here with one is a compiler bug and says so.
 *
 * NOT `qsort`: `qsort` is unstable and platform-dependent, and the M-selfhost-fixpoint fixpoint
 * compares generated C byte for byte. NaN in an `[f64]` aborts rather than being
 * given an invented place. */
HeroArrayHeader *hero_array_sort(const HeroArrayHeader *a);

/* `chars(s) -> [str]` — one `str` per character, TOTAL over every byte string: a
 * byte that starts no well-formed UTF-8 sequence becomes a one-byte `str`. The
 * law that buys, and the reason it is not an abort:
 *     join(chars(s), "") == s   for every s.
 * Well-formedness is enforced at `hero_str_slice`, the one operation that can
 * break it — not here, which is only where the symptom would show. */
HeroArrayHeader *hero_str_chars(HeroStr s);

/* `join(parts, sep)` — one allocation, design.md:1318's answer to O(n^2)
 * concatenation. `parts` must hold `str`; anything else is a compiler bug. */
HeroStr hero_str_join(const HeroArrayHeader *parts, HeroStr sep);

/* -- copy-on-write (panel 022, and the veto that shaped it) ------------------
 *
 * `xs[i] @ v` mutates a place, and the place may be shared. So: make it unique
 * first, and do it ONCE PER ARRAY STEP of the place path, each writing back at
 * its own level. `**` rather than a returned pointer is what makes the write-back
 * unavoidable — a caller cannot forget to store the result if there is no result.
 *
 * WHY PER STEP, measured: with a single unshare at the primitive, `h = g` then
 * `g.rows[0].cells[0] @ 7` changes `h` too — ASan clean, leak counter zero, exit
 * 0. A green harness on a program that violates spec line 60 ("No aliasing exists
 * anywhere"). It is *necessarily* wrong, not accidentally: unsharing level 1
 * copies its elements, whose `copy` increfs level 2, so level 2 is shared exactly
 * when level 1 was copied.
 *
 * WHY NOT AN IR INSTRUCTION: as a `cow_check` op inserted before every mutation it
 * can be hoisted above the argument, and in that order the unshare is a no-op and
 * `xs` ends up reaching itself — a three-block cycle a judge actually built, with
 * ASan silent and only the block counter catching it. Inside the primitive, C's
 * own rule that every argument is evaluated before the callee's first statement
 * makes the bad order INEXPRESSIBLE, which beats a check because a check can be
 * forgotten. */

void hero_array_unshare(HeroArrayHeader **slot);

/* The address of one element, for descending a place path after unsharing.
 * Aborts out of range. Non-const, unlike `hero_array_at`, because the caller has
 * just made the block its own. */
void *hero_array_at_mut(HeroArrayHeader *a, int64_t index);

/* Replace one element. Unshares first, releases the element that was there, and
 * MOVES the value in — the caller hands over a reference (+1) rather than lending
 * one, which is why the ownership pass increfs before the store and does not
 * decref after. The order is part of the rule: the incref happens BEFORE the
 * outermost unshare, because the value may live inside the very container being
 * copied (`n.children[0] @ n`). */
void hero_array_set(HeroArrayHeader **slot, int64_t index, const void *value);

/* The place store: `p @ p.push(v)` on a bare place, recognised at lowering
 * (panels 037 and 088; landed by author decision 2026-08-24). `**` and not a
 * returned pointer, for hero_array_set's reason — and because the slot is what
 * makes uniqueness knowable: the classic call-and-store shape increfs its
 * arguments, so `hero_array_push` can never see a true count (panel 037: the
 * gate fired on 0 of 100,000 accumulator pushes). Here the emitter passes the
 * place itself; refcount 1 with room appends in place, anything else copies
 * into a geometrically grown block. The value is BORROWED (copied in, like
 * `hero_array_push`), never moved — and the copy is guarded: copying the value
 * may incref this very array (`n.kids @ n.kids.push(n)`), and committing the
 * longer length then would nest the array inside itself, so the refcount is
 * re-read across the copy and a moved count undoes and takes the copying path,
 * which snapshots. */
void hero_array_push_owned(HeroArrayHeader **slot, const void *value);

/* -- the map: `{K: V}` (design.md §4.20, panels 006 and 022) -----------------
 *
 * Open addressing with linear probing, and THREE PARALLEL REGIONS in one block —
 * a state byte per bucket, then the keys, then the values. Not one struct per
 * bucket, because key and value sizes come from descriptors at runtime: a
 * per-bucket layout would need padding this code computes, while parallel regions
 * need only each region aligned once. The offsets are in the header so nothing
 * recomputes them.
 *
 * FIXED SEED, and it is not a style choice: iteration order has to be a function
 * of the contents, because the self-hosting fixpoint compares generated C
 * byte-for-byte (panel 006). A seed that varied per run would make the compiler
 * produce two different correct outputs, and the fixpoint would never close.
 *
 * The mutation half landed at M-generics-library step 2 (panel 026), and it is a **place store**
 * rather than a `set(m, k, v)` call: `m[k] @ v` needed no new IR form, where a
 * void builtin would have grown a special case in the checker, the ownership pass
 * and the emitter. Iteration is `keys(m)` composed with the `for` and `sort` that
 * already existed, so the language grew no loop form either. There is still no
 * `hero_map_unshare`: a map is copied, not unshared, because nothing writes
 * through a shared one — `m[k] @ v` goes through `hero_map_set` on the slot. */

typedef struct HeroMapHeader {
    HeroRefcount refcount;
    int64_t len;  /* live entries */
    int64_t cap;  /* buckets, always a power of two, always > len */
    const HeroDesc *key;
    const HeroDesc *val;
    size_t states; /* byte offsets from this header to the three regions */
    size_t keys;
    size_t vals;
} HeroMapHeader;

extern const HeroDesc hero_desc_map;

/* `cap` is rounded up to a power of two with room to spare: linear probing
 * degrades badly at high load, and a literal's size is known exactly, so there is
 * no reason to be tight. */
HeroMapHeader *hero_map_new(const HeroDesc *key, const HeroDesc *val, int64_t entries);
void hero_map_incref(HeroMapHeader *m);
void hero_map_decref(HeroMapHeader *m); /* no-op on NULL */
int64_t hero_map_len(const HeroMapHeader *m);

/* Used only by the literal builder. It COPIES both key and value through their
 * descriptors — the same rule as `hero_array_push`, because every value reaching a
 * constructor is borrowed. A duplicate key replaces the VALUE, since `{"a": 1,
 * "a": 2}` has to mean something and the later entry is what a reader expects. */
void hero_map_put(HeroMapHeader *m, const void *key, const void *value);

/* `keys(m) -> [K]` — a fresh array of the keys, in an order the spec declines to
 * promise (panel 026). Iteration is this composed with the `for` and `sort` that
 * already exist, which is why the language grew no loop form for a map: two
 * invocations compose to it (CLAUDE.md §10).
 *
 * The keys are COPIED through the key descriptor, so the array owns its own
 * references and outliving the map is safe. */
HeroArrayHeader *hero_map_keys(const HeroMapHeader *m);

/* `m[k] @ v` — insert or replace. Named `set` rather than `put` because `put` is
 * the literal builder's private entry point and this is the language's.
 *
 * Unlike `hero_array_set` this one **grows**: a store may add an entry, so the
 * table can fill, and unlike an array a map has no "index out of range" — spec
 * says `xs[i] @ v` aborts when absent and `m[k] @ v` inserts. It takes `**` for
 * the same reason every mutation primitive does: growth replaces the block, and a
 * caller cannot forget to store a result that does not exist. */
void hero_map_set(HeroMapHeader **slot, const void *key, const void *value);

/* The value, or NULL when the key is absent. NULL rather than a `T?` because the
 * runtime cannot build one: the option struct is generated per payload type, so
 * wrapping is the emitter's job. */
const void *hero_map_find(const HeroMapHeader *m, const void *key);

/* Order-independent by construction: same length, and every entry of `a` found in
 * `b` with an equal value. Spec line 58 requires exactly that — "a map's insertion
 * order does not affect it" — and a pairwise walk of two dense entry arrays would
 * have made `{"a":1,"b":2}` and `{"b":2,"a":1}` unequal, which is the shape panel
 * 022 ranked first among the things a cheap implementation gets silently wrong. */
bool hero_map_eq(const HeroMapHeader *a, const HeroMapHeader *b);

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
