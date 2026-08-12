/* Spike 4 — THE DECIDING SPIKE: a recursive variant with a refcounted array,
 * its per-type functions, exercised under AddressSanitizer.
 *
 * This spike fixes two decisions before any compiler code exists:
 *
 *   1. THE CONTAINER REPRESENTATION. `[T]` is a pointer to a heap header
 *      (refcount, len, cap, element descriptor) followed by the elements
 *      in-line. The array is Heroes' ONLY indirection (design.md §4.10), which
 *      is what makes the recursive type below finite.
 *
 *   2. THE DESCRIPTOR ABI. C has no copy constructors, destructors, or generic
 *      equality — but design.md demands structural `==` on everything (§4.3),
 *      value-semantics copies, and drops. So the compiler will GENERATE, for
 *      every reachable type, plain C functions (h_T_copy / h_T_drop / h_T_eq)
 *      and a descriptor struct pointing at them. The runtime works through
 *      descriptors; clang still type-checks every call.
 *
 * The Heroes type this hand-compiles (design.md §4.10's own example):
 *
 *     variant Expr
 *         num
 *             v: int
 *         sum
 *             children: [Expr]
 *
 * What main() does: builds 1 + (2 + 3), deep-copies it, checks structural
 * equality, mutates the copy, checks inequality, drops everything.
 *
 * Build (sanitizers are the point of this spike):
 *   clang -std=c11 -Wall -Werror=return-type -fsanitize=address,undefined \
 *       -Iruntime tools/spike/04-variant.c runtime/runtime.c -o build/spike04
 * Run:  build/spike04     → three lines: 1, 1, -42 (checked against
 *                          tools/spike/04-variant.expected by a test) — and ASan must
 *       report ZERO leaks (it aborts loudly if a drop is missing).
 */

#include "heroes_runtime.h"

#include <stdlib.h>
#include <string.h>

/* -- the descriptor ABI and the array now SHIP (M-value-aggregates step 5) ------------------
 *
 * This spike declared `HeroDesc` and `HeroArrayHeader` itself, plus a hand-rolled
 * `hero_array_*`, because it ran before any of it existed — that was the point:
 * fix the representation before writing the compiler. Panel 022 scheduled the
 * move, and M-value-aggregates step 5 made it: both types and every array primitive are in
 * `heroes_runtime.h`, so keeping local copies here is now
 * `error: redefinition of 'HeroDesc'`.
 *
 * What the spike still owns is the part it was built to prove: the per-type
 * functions for a recursive variant, hand-written where the compiler now
 * generates them. Comparing this file to `heroes build --emit-c` on the same type
 * is the check that the generator and the frozen decision still agree.
 *
 * One behaviour changed with the move, and it is recorded rather than hidden: the
 * spike's own `hero_array_push` MOVED its element and asserted refcount == 1,
 * while the shipped one COPIES — `push(xs, v)` must leave `xs` observable, so
 * value semantics has no reading in which the argument is consumed. So the
 * pushes below hand over borrowed values and the locals are dropped after.
 */

/* -- the generated shape for `Expr` (hand-written here, M-value-aggregates generates it) -- */

typedef enum { H_EXPR_NUM, H_EXPR_SUM } h_Expr_tag;

typedef struct {
    h_Expr_tag tag;
    union {
        struct { int64_t v; } num;
        struct { HeroArrayHeader *children; } sum; /* [Expr] — the indirection */
    } as;
} h_Expr;

static const HeroDesc h_Expr_desc; /* forward: elements reference their type */

/* SHALLOW plus incref, amended at M-value-aggregates by panel 022.
 *
 * This function used to deep-copy, and that was the real contradiction between this
 * spike and the descriptor pass — not the variant's boxing, which the spike had
 * right all along. Copy-on-write is what makes a deep copy unnecessary: sharing is
 * unobservable until somebody mutates, and the mutation primitives unshare. Deep
 * copying here would pay for every binding what only a mutation costs.
 *
 * The consequence is visible in this spike's own output, which is why it has a
 * checked `.expected`: the copy now shares its children, so mutating the original's
 * array is observable through the copy *within this file*, which has no COW at the
 * mutation site. The second printed line changes from 0 to 1 and that change is the
 * amendment, not a regression. A real M-value-aggregates program cannot see it. */
static void h_Expr_copy(void *dst_v, const void *src_v) {
    const h_Expr *src = src_v;
    h_Expr *dst = dst_v;
    *dst = *src;
    if (src->tag == H_EXPR_SUM) {
        hero_array_incref(src->as.sum.children);
    }
}

static void h_Expr_drop(void *e_v) {
    h_Expr *e = e_v;
    if (e->tag == H_EXPR_SUM) hero_array_decref(e->as.sum.children);
}

static bool h_Expr_eq(const void *a_v, const void *b_v) {
    const h_Expr *a = a_v;
    const h_Expr *b = b_v;
    if (a->tag != b->tag) return false;
    switch (a->tag) {
        case H_EXPR_NUM: return a->as.num.v == b->as.num.v;
        case H_EXPR_SUM: return hero_array_eq(a->as.sum.children, b->as.sum.children);
    }
    hero_unreachable(); /* exhaustive switch; reaching here is a compiler bug */
}

/* Added when the array moved into the runtime, and NOT because the spike needed
 * it: `hero_array_new` refuses a descriptor whose `hash` is null, which is panel
 * 022's rule ("generated for every type, never null — a call through a null one
 * is SEGV at pc 0x0, with no type name and no source line"). The rule caught this
 * file on its first run against the shipped header, which is the best evidence
 * that a check with an instrument beats a check in a comment.
 *
 * Fields, never bytes, for the same reason `eq` is: the union means the padding
 * after a `num` payload is whatever the last `sum` left there. */
static uint64_t h_Expr_hash(const void *e_v) {
    const h_Expr *e = e_v;
    uint64_t h = (uint64_t)e->tag;
    switch (e->tag) {
        case H_EXPR_NUM:
            return h * UINT64_C(0x100000001b3) ^ (uint64_t)e->as.num.v;
        case H_EXPR_SUM:
            return h * UINT64_C(0x100000001b3) ^ hero_desc_array.hash(&e->as.sum.children);
    }
    hero_unreachable();
}

static const HeroDesc h_Expr_desc = {
    .size = sizeof(h_Expr),
    .copy = h_Expr_copy,
    .drop = h_Expr_drop,
    .eq = h_Expr_eq,
    .hash = h_Expr_hash,
};

/* -- convenience constructors (the emitter inlines these shapes) ----------- */

static h_Expr expr_num(int64_t v) {
    return (h_Expr){.tag = H_EXPR_NUM, .as.num = {v}};
}

/* sum of exactly two children — enough for the spike.
 *
 * `hero_array_push` COPIES, so each push takes its own reference and the local
 * `a`/`b` still own theirs: they are dropped here, which is exactly what the
 * ownership pass emits for a temporary that has been handed to a constructor. */
static h_Expr expr_sum2(h_Expr a, h_Expr b) {
    HeroArrayHeader *kids = hero_array_new(&h_Expr_desc, 2);
    HeroArrayHeader *with_a = hero_array_push(kids, &a);
    hero_array_decref(kids);
    HeroArrayHeader *with_b = hero_array_push(with_a, &b);
    hero_array_decref(with_a);
    h_Expr_drop(&a);
    h_Expr_drop(&b);
    return (h_Expr){.tag = H_EXPR_SUM, .as.sum = {with_b}};
}

int main(void) {
    /* tree = 1 + (2 + 3) */
    h_Expr tree = expr_sum2(expr_num(1), expr_sum2(expr_num(2), expr_num(3)));

    /* COW preview: a plain `b = a` in Heroes is incref-only — the deep copy
     * below is what the COW machinery performs at first mutation (M-value-aggregates). */
    hero_array_incref(tree.as.sum.children);
    hero_array_decref(tree.as.sum.children);

    /* copy = an independent deep copy (Heroes: `copy = tree` + mutation) */
    h_Expr copy;
    h_Expr_copy(&copy, &tree);

    /* structural equality, recursively (§4.3) */
    hero_print_int(h_Expr_eq(&tree, &copy) ? 1 : 0); /* → 1 */
    hero_print_end();

    /* mutate the copy's inner leaf; the original must be untouched */
    const h_Expr *inner = (const h_Expr *)hero_array_at(copy.as.sum.children, 1);
    /* Written through a const pointer on purpose: the shipped reader is
     * `hero_array_at`, which hands back `const void *` because a Heroes program
     * mutates through a place and never through an element pointer. The cast is
     * this file admitting it is hand-compiling something the emitter would not. */
    h_Expr *leaf = (h_Expr *)(void *)hero_array_at(inner->as.sum.children, 0);
    leaf->as.num.v = 99;
    /* → 1 since M-value-aggregates: the copy SHARES this array, and this file has no COW at the
     * mutation site, so the mutation is visible through both. A real Heroes program
     * cannot see it — `xs[i] @ v` unshares first, per step. */
    hero_print_int(h_Expr_eq(&tree, &copy) ? 1 : 0); /* → 1 */
    hero_print_end();

    /* drop both — ASan verifies zero leaks, zero double-frees */
    h_Expr_drop(&tree);
    h_Expr_drop(&copy);

    hero_print_int(-42); /* "ok" sentinel: reached the end alive */
    hero_print_end();
    return 0;
}
