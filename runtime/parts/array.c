/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/array.c — `[T]`, and the operations that do not mutate.
 *
 * A pointer to a heap header followed by the elements IN-LINE. The array is
 * Heroes' only indirection (§4.10), which is what gives a recursive type a
 * finite size, and being one pointer wide is why an array field imposes no
 * ordering constraint on the generated C.
 *
 * Everything here works through the element descriptor, so nothing in this file
 * knows what the array holds. The mutating half is `parts/cow.c`, kept apart
 * because copy-on-write is the subtlest rule in this runtime.
 *
 * design.md §4.10, §4.20, spike 04's frozen ABI.
 */

/* Declared here, defined after the array functions they call. */
static void hero_copy_array(void *dst, const void *src);
static void hero_drop_array(void *elem);
static bool hero_eq_array(const void *a, const void *b);
static uint64_t hero_hash_array(const void *elem);

const HeroDesc hero_desc_array = {sizeof(HeroArrayHeader *), hero_copy_array,
                                  hero_drop_array, hero_eq_array, hero_hash_array};

static unsigned char *hero_array_data(HeroArrayHeader *a) {
    return (unsigned char *)(void *)(a + 1);
}
static const unsigned char *hero_array_data_const(const HeroArrayHeader *a) {
    return (const unsigned char *)(const void *)(a + 1);
}

/* Every reader goes through this: NULL is an unassigned or moved-out slot, and
 * reading one is a compiler bug, not an empty array. */
static void hero_array_require(const HeroArrayHeader *a) {
    if (a == NULL) {
        hero_panic("read of an unassigned array slot — this is a compiler bug, please report it");
    }
}

HeroArrayHeader *hero_array_new(const HeroDesc *elem, int64_t cap) {
    if (elem == NULL) hero_panic("array with no element descriptor — a compiler bug");
    if (elem->hash == NULL) hero_panic("element descriptor with no hash — a compiler bug");
    if (cap < 1) cap = 1;
    if ((uint64_t)cap > (SIZE_MAX - sizeof(HeroArrayHeader)) / elem->size) {
        hero_panic("array too large");
    }
    HeroArrayHeader *a = hero_alloc_block(sizeof(HeroArrayHeader) + (size_t)cap * elem->size);
    /* Relaxed on a block no other thread can see yet — `str.c`'s reason. */
    atomic_store_explicit(&a->refcount, 1, memory_order_relaxed);
    a->len = 0;
    a->cap = cap;
    a->elem = elem;
    return a;
}

/* Relaxed up, acquire-release down — `str.c`'s two paragraphs carry the reason,
 * and it is the same reason here because it is the same counter type. */
void hero_array_incref(HeroArrayHeader *a) {
    if (a == NULL) return;
    atomic_fetch_add_explicit(&a->refcount, 1, memory_order_relaxed);
}

/* Releasing an array releases what it holds, **iteratively** — see `drop.c` for
 * why, and for the depth at which the recursion this replaces ran out of stack.
 *
 * Whichever of the two release entry points is called first becomes the driver;
 * every release reached from inside it hands its block over and returns. The
 * driver then drains both lists until both are empty, so a value alternating
 * arrays and maps unwinds without a frame per level. */
void hero_array_decref(HeroArrayHeader *a) {
    if (a == NULL) return; /* the zero-init non-value: a no-op */
    /* `> 1` on the count BEFORE the subtraction is exactly the old `> 0` on the
     * count after it, negative counts included — and a doomed block must reach
     * the drop list on the path that used to read a negative, not be returned
     * from as if it were still shared. */
    if (atomic_fetch_sub_explicit(&a->refcount, 1, memory_order_acq_rel) > 1) return;
    if (hero_drop_running) {
        hero_drop_push_array(a);
        return;
    }
    hero_drop_running = true;
    hero_drop_drain_from_array(a);
    hero_drop_running = false;
}

/* The walk `drop.c`'s drainer calls. Releases the elements and then the block,
 * and never recurses: an element that is itself a container sees
 * `hero_drop_running` raised and defers. */
static void hero_array_release_contents(HeroArrayHeader *a) {
    unsigned char *data = hero_array_data(a);
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->drop(data + (size_t)i * a->elem->size);
    }
    hero_release_block(a);
}

int64_t hero_array_len(const HeroArrayHeader *a) {
    hero_array_require(a);
    return a->len;
}

const void *hero_array_at(const HeroArrayHeader *a, int64_t index) {
    hero_array_require(a);
    if (index < 0 || index >= a->len) hero_panic("array index out of range");
    return hero_array_data_const(a) + (size_t)index * a->elem->size;
}

HeroArrayHeader *hero_array_push(const HeroArrayHeader *a, const void *elem) {
    hero_array_require(a);
    if (a->len == INT64_MAX) hero_panic("array length overflow");
    HeroArrayHeader *b = hero_array_new(a->elem, a->len + 1);
    const unsigned char *src = hero_array_data_const(a);
    unsigned char *dst = hero_array_data(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->copy(dst + (size_t)i * size, src + (size_t)i * size);
    }
    /* The new element is copied in, not moved: the caller's value is borrowed
     * (every value reaching a call is, by the ownership pass's rule 5), so the
     * array takes its own reference exactly as it does for the ones it copied. */
    a->elem->copy(dst + (size_t)a->len * size, elem);
    b->len = a->len + 1;
    return b;
}

/* `==` on two arrays is a function of their CONTENTS and of nothing else.
 *
 * There used to be `if (a == b) return true;` here, and deleting it is panel
 * 069's whole resolution. Copy-on-write makes `b = a` share a header, so that
 * line made the answer depend on how the second value was PRODUCED rather than
 * on what it holds: with a nan inside, `a == b` was true and `a == c` was false
 * for identical contents. spec § 3 Types promises "no aliasing exists anywhere", and
 * the line made copy-on-write's sharing observable — so the shortcut was not a
 * float bug, it was an aliasing leak that a float happened to expose.
 *
 * The sitting proved that with no float in the program at all: with panel 061's
 * partial-record refusal reachable, the shared operands returned `true` at exit 0
 * while the distinct ones hit the loud `hero_panic` that refusal exists to fire.
 * An optimisation that silently defeats a deliberate loud failure is the exact
 * inversion of CLAUDE.md §11's rule about which direction a fallback points.
 *
 * The two defences the line has everywhere else both fail here. SPEED is
 * forbidden as a justification by CLAUDE.md §13 — and measured, the golden
 * harness got 3% FASTER without it, because the walk it skipped was never on the
 * hot path. TERMINATION on a cyclic value is Python's reason and needs cycles:
 * design.md:2855 makes them unconstructible ("if values are never aliased,
 * reference cycles cannot be constructed... that is not a compromise, it is the
 * reason the whole design is small"), measured both ways in the sitting.
 *
 * The length check above it stays: two arrays of different lengths differ in
 * their contents, which is a fact about the values and not about their addresses. */
/* **`==` is a conjunction, and that is what makes an unbounded comparison cost no
 * emitter line at all** (panel 076, prototyped independently by two seats).
 *
 * A deep value used to SEGFAULT here: `Node { children: [Node] }` built a chain,
 * `hero_array_eq` called the element's `eq`, the generated `_eq` called back into
 * `hero_array_eq`, one C frame per level, exit 139 with no output. The cliff was
 * ~52 200 at `-O0` and it is not a property of the program — the same binary at
 * the same depth exits 0 in a plain environment and 139 with one 900 KB
 * environment variable set, because the environment is copied onto the stack. So
 * no depth NUMBER could ever have been honest, and panel 076 refused one.
 *
 * What lands instead needs no continuation, which is why the compiler-engineer's
 * standing veto on rewriting `emit/structural.rs` is untouched by it: **`true` is
 * the identity of `&&`**. A nested call may push its elementwise work, answer
 * `true` provisionally — *nothing unequal yet, the rest is queued* — and let the
 * OUTERMOST call drain the queue and AND the real answer in. The generated `_eq`
 * never learns it was deferred, so the Heroes port reproduces nothing.
 *
 * It is sound because the checker makes it so, measured rather than assumed:
 * `record Node { child: Node }`, `child: Node?` and a self-referencing variant are
 * all `error[no_size]`. **Every unbounded descent goes through `[T]` or `{K: V}`,
 * and both live here.**
 *
 * One behaviour change is adopted with it and stated rather than discovered: a
 * generated `_eq` that PANICS — panel 061's `partial` refusal — can now fire on a
 * subtree the old short-circuit would have skipped. */
typedef struct {
    const unsigned char *a;
    const unsigned char *b;
    const HeroDesc *elem;
    int64_t len;
} HeroEqWork;

/* **The expiry date came, and this is what the comment used to promise.** It read
 * *"these three are file-static rather than thread-local, and that is a decision
 * with an expiry date rather than an oversight … the day it arrives, this queue
 * is shared mutable state across threads and must become `_Thread_local` (the
 * buffer then leaks one allocation per thread, which is why it is not that
 * already)"*. Both halves were right, and both are settled at
 * M-isolated-threads step 4.
 *
 * The four are `_Thread_local` now, and the parenthesis — the reason they were
 * not — is answered where it belongs rather than here: `parts/alloc.c` records
 * this thread's kept buffer in a key whose destructor frees it, so the thread
 * that grew the queue is the one that gives it back. That question is the
 * allocator's by §4.20 and by that file's own words, and the array only has to
 * say what it needs. `hero_hash_depth` below was thread-local from the start,
 * because a counter costs nothing to make so. */
static _Thread_local HeroEqWork *hero_eq_queue = NULL;
static _Thread_local size_t hero_eq_len = 0;
static _Thread_local size_t hero_eq_cap = 0;
static _Thread_local bool hero_eq_running = false;

static void hero_eq_push(const unsigned char *a, const unsigned char *b, const HeroDesc *elem,
                         int64_t len) {
    if (hero_eq_len == hero_eq_cap) {
        size_t cap = hero_eq_cap ? hero_eq_cap * 2 : 64;
        /* The guard before the allocation, §4.20's normative sentence: the
         * doubling is what could wrap, and a wrapped size allocates a few bytes
         * and is then written past. `hero_malloc_raw` says "out of memory" for a
         * refused malloc, as it does everywhere else; this is the case worth its
         * own words, because it is the one this call can produce. */
        if (cap > SIZE_MAX / sizeof(HeroEqWork)) hero_panic("out of memory comparing a deep value");
        /* Through the allocator, never `realloc`: design.md Part 7.13 has one
         * allocation point, and this queue is exactly the state that becomes
         * `_Thread_local` the day threads arrive. */
        hero_eq_queue = (HeroEqWork *)hero_grow_kept(hero_eq_queue, hero_eq_cap * sizeof(HeroEqWork),
                                                     cap * sizeof(HeroEqWork));
        hero_eq_cap = cap;
    }
    hero_eq_queue[hero_eq_len].a = a;
    hero_eq_queue[hero_eq_len].b = b;
    hero_eq_queue[hero_eq_len].elem = elem;
    hero_eq_queue[hero_eq_len].len = len;
    hero_eq_len++;
}

bool hero_array_eq(const HeroArrayHeader *a, const HeroArrayHeader *b) {
    hero_array_require(a);
    hero_array_require(b);
    if (a->len != b->len) return false;
    const unsigned char *da = hero_array_data_const(a);
    const unsigned char *db = hero_array_data_const(b);
    /* A nested call defers and answers provisionally; only the outermost drains. */
    if (hero_eq_running) {
        hero_eq_push(da, db, a->elem, a->len);
        return true;
    }
    hero_eq_running = true;
    size_t base = hero_eq_len;
    hero_eq_push(da, db, a->elem, a->len);
    bool equal = true;
    while (equal && hero_eq_len > base) {
        HeroEqWork work = hero_eq_queue[--hero_eq_len];
        size_t size = work.elem->size;
        for (int64_t i = 0; i < work.len; i++) {
            if (!work.elem->eq(work.a + (size_t)i * size, work.b + (size_t)i * size)) {
                equal = false;
                break;
            }
        }
    }
    /* Whatever the answer, the queue must come back to where this call found it:
     * an early `false` leaves work behind that belongs to nobody. */
    hero_eq_len = base;
    hero_eq_running = false;
    return equal;
}

/* `slice(xs, from:, to:)` on an array — a NEW array, elements copied through the
 * descriptor, and it ABORTS out of range with the same three-part test and the
 * same shape of message as `hero_str_slice`.
 *
 * Abort rather than clamp (panel 027 R3): a clamping slice hands a shorter array
 * to whatever comes next, and when that next thing is a length passed to C —
 * §1.11's whole point — the mismatch is silent. Go, Rust's indexing form and Zig
 * all abort; Python clamps, and nobody has ever documented what that cost. */
HeroArrayHeader *hero_array_slice(const HeroArrayHeader *a, int64_t from, int64_t to) {
    hero_array_require(a);
    if (from < 0 || to < from || to > a->len) hero_panic("array slice out of range");
    int64_t n = to - from;
    HeroArrayHeader *b = hero_array_new(a->elem, n);
    const unsigned char *src = hero_array_data_const(a);
    unsigned char *dst = hero_array_data(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < n; i++) {
        a->elem->copy(dst + (size_t)i * size, src + (size_t)(from + i) * size);
    }
    b->len = n;
    return b;
}

/* One descriptor for every `[T]`: these four reach the element type through the
 * header, so nothing here depends on what the array holds. */
static void hero_copy_array(void *dst, const void *src) {
    HeroArrayHeader *a = *(HeroArrayHeader *const *)src;
    hero_array_incref(a);
    *(HeroArrayHeader **)dst = a;
}
static void hero_drop_array(void *elem) { hero_array_decref(*(HeroArrayHeader **)elem); }
static bool hero_eq_array(const void *a, const void *b) {
    return hero_array_eq(*(HeroArrayHeader *const *)a, *(HeroArrayHeader *const *)b);
}
/* An array is hashable so that a descriptor's `hash` is never null (panel 022),
 * not because an array can be a map key — `{[i64]: v}` is a question §4.9 has
 * not answered. Order matters, because `==` on an array is order-sensitive. */
/* **`hash` takes the OPPOSITE fix to `eq`, and the asymmetry is the point** (panel
 * 076, the compiler-engineer's own contribution).
 *
 * `eq` must be exact, so it got a worklist and no bound. `hash` may lose
 * information: its only contract is that equal values hash equal, and a hash that
 * stops descending after a fixed number of levels still keeps it — every value
 * that agrees to that depth simply shares a bucket, and `eq` tells them apart
 * afterwards. So a **cap** is admissible here where it was refused for `eq`.
 *
 * And it is admissible for the reason CLAUDE.md §11 gives: this number is a fact
 * about **the value** — how deep a hash bothers to look — not a claim about the
 * machine's stack, which moves with `-O`, with `ulimit` and even with the size of
 * the environment. Being wrong costs collisions. It can never cost a crash, and it
 * cannot expire in silence. */
#define HERO_HASH_MAX_DEPTH 128

static _Thread_local int64_t hero_hash_depth = 0;

static uint64_t hero_hash_array(const void *elem) {
    const HeroArrayHeader *a = *(const HeroArrayHeader *const *)elem;
    hero_array_require(a);
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    if (hero_hash_depth >= HERO_HASH_MAX_DEPTH) return h;
    hero_hash_depth++;
    const unsigned char *data = hero_array_data_const(a);
    for (int64_t i = 0; i < a->len; i++) {
        uint64_t one = a->elem->hash(data + (size_t)i * a->elem->size);
        h ^= one;
        h *= UINT64_C(0x100000001b3);
    }
    hero_hash_depth--;
    return h;
}
