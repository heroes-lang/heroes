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
 * Run:  build/spike04     → three lines: 1, 0, -42 (checked against
 *                          tools/spike/04-variant.expected by a test) — and ASan must
 *       report ZERO leaks (it aborts loudly if a drop is missing).
 */

#include "heroes_runtime.h"

#include <stdlib.h>
#include <string.h>

/* -- the descriptor ABI (to be generated per reachable type in M5c) -------- */

typedef struct HeroDesc HeroDesc;
struct HeroDesc {
    size_t size;                              /* element size in bytes        */
    void (*copy)(void *dst, const void *src); /* deep copy (incref inside)    */
    void (*drop)(void *elem);                 /* release owned memory         */
    bool (*eq)(const void *a, const void *b); /* structural equality (§4.3)   */
};

/* -- the array representation ([T]) ---------------------------------------- */

typedef struct {
    int64_t refcount;
    int64_t len;
    int64_t cap;
    const HeroDesc *elem;
    /* elements follow in-line: len * elem->size bytes */
} HeroArrayHeader;

static void *hero_array_data(HeroArrayHeader *a) { return (void *)(a + 1); }

static HeroArrayHeader *hero_array_new(const HeroDesc *elem, int64_t cap) {
    if (cap < 1) cap = 1;
    HeroArrayHeader *a =
        malloc(sizeof(HeroArrayHeader) + (size_t)cap * elem->size);
    if (a == NULL) hero_panic("out of memory");
    a->refcount = 1;
    a->len = 0;
    a->cap = cap;
    a->elem = elem;
    return a;
}

static void hero_array_incref(HeroArrayHeader *a) { a->refcount += 1; }

static void hero_array_decref(HeroArrayHeader *a) {
    a->refcount -= 1;
    if (a->refcount > 0) return;
    unsigned char *data = hero_array_data(a);
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->drop(data + (size_t)i * a->elem->size);
    }
    free(a);
}

/* push MOVES the element in (ownership transfers to the array).
 * COW (copy when refcount > 1) arrives in M5c; this spike only needs the
 * unshared path, and asserts that precondition instead of hiding it. */
static HeroArrayHeader *hero_array_push(HeroArrayHeader *a, const void *elem) {
    if (a->refcount != 1) hero_panic("push on shared array: COW is M5c work");
    if (a->len == a->cap) {
        int64_t cap2 = a->cap * 2;
        HeroArrayHeader *b =
            realloc(a, sizeof(HeroArrayHeader) + (size_t)cap2 * a->elem->size);
        if (b == NULL) hero_panic("out of memory");
        a = b;
        a->cap = cap2;
    }
    memcpy((unsigned char *)hero_array_data(a) + (size_t)a->len * a->elem->size,
           elem, a->elem->size);
    a->len += 1;
    return a;
}

static bool hero_array_eq(const HeroArrayHeader *a, const HeroArrayHeader *b) {
    if (a->len != b->len) return false;
    const unsigned char *da = (const unsigned char *)(a + 1);
    const unsigned char *db = (const unsigned char *)(b + 1);
    for (int64_t i = 0; i < a->len; i++) {
        size_t off = (size_t)i * a->elem->size;
        if (!a->elem->eq(da + off, db + off)) return false;
    }
    return true;
}

/* Deep copy = new array + per-element copy. Under full COW semantics a plain
 * `b = a` is incref-only and copying happens on mutation; the deep path below
 * is what the COW machinery calls at that point. */
static HeroArrayHeader *hero_array_deep_copy(HeroArrayHeader *a) {
    HeroArrayHeader *b = hero_array_new(a->elem, a->cap);
    const unsigned char *src = (const unsigned char *)(a + 1);
    unsigned char *dst = hero_array_data(b);
    for (int64_t i = 0; i < a->len; i++) {
        size_t off = (size_t)i * a->elem->size;
        a->elem->copy(dst + off, src + off);
    }
    b->len = a->len;
    return b;
}

/* -- the generated shape for `Expr` (hand-written here, M5c generates it) -- */

typedef enum { H_EXPR_NUM, H_EXPR_SUM } h_Expr_tag;

typedef struct {
    h_Expr_tag tag;
    union {
        struct { int64_t v; } num;
        struct { HeroArrayHeader *children; } sum; /* [Expr] — the indirection */
    } as;
} h_Expr;

static const HeroDesc h_Expr_desc; /* forward: elements reference their type */

static void h_Expr_copy(void *dst_v, const void *src_v) {
    const h_Expr *src = src_v;
    h_Expr *dst = dst_v;
    *dst = *src;
    if (src->tag == H_EXPR_SUM) {
        /* value semantics: the copy owns its own tree */
        dst->as.sum.children = hero_array_deep_copy(src->as.sum.children);
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

static const HeroDesc h_Expr_desc = {
    .size = sizeof(h_Expr),
    .copy = h_Expr_copy,
    .drop = h_Expr_drop,
    .eq = h_Expr_eq,
};

/* -- convenience constructors (the emitter inlines these shapes) ----------- */

static h_Expr expr_num(int64_t v) {
    return (h_Expr){.tag = H_EXPR_NUM, .as.num = {v}};
}

/* sum of exactly two children — enough for the spike */
static h_Expr expr_sum2(h_Expr a, h_Expr b) {
    HeroArrayHeader *kids = hero_array_new(&h_Expr_desc, 2);
    kids = hero_array_push(kids, &a); /* moves a */
    kids = hero_array_push(kids, &b); /* moves b */
    return (h_Expr){.tag = H_EXPR_SUM, .as.sum = {kids}};
}

int main(void) {
    /* tree = 1 + (2 + 3) */
    h_Expr tree = expr_sum2(expr_num(1), expr_sum2(expr_num(2), expr_num(3)));

    /* COW preview: a plain `b = a` in Heroes is incref-only — the deep copy
     * below is what the COW machinery performs at first mutation (M5c). */
    hero_array_incref(tree.as.sum.children);
    hero_array_decref(tree.as.sum.children);

    /* copy = an independent deep copy (Heroes: `copy = tree` + mutation) */
    h_Expr copy;
    h_Expr_copy(&copy, &tree);

    /* structural equality, recursively (§4.3) */
    hero_print_int(h_Expr_eq(&tree, &copy) ? 1 : 0); /* → 1 */
    hero_print_end();

    /* mutate the copy's inner leaf; the original must be untouched */
    h_Expr *inner =
        &((h_Expr *)hero_array_data(copy.as.sum.children))[1];
    ((h_Expr *)hero_array_data(inner->as.sum.children))[0].as.num.v = 99;
    hero_print_int(h_Expr_eq(&tree, &copy) ? 1 : 0); /* → 0 */
    hero_print_end();

    /* drop both — ASan verifies zero leaks, zero double-frees */
    h_Expr_drop(&tree);
    h_Expr_drop(&copy);

    hero_print_int(-42); /* "ok" sentinel: reached the end alive */
    hero_print_end();
    return 0;
}
