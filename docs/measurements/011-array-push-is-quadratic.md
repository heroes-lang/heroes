# 011 — `xs @ xs.push(v)` is quadratic, and it is the language's commonest line

Date: 2026-08-23. Found while writing a harness scanner: `strings.lines` over
`seed/heroes.c` (22 MB, 724k lines) **never finished** — over ten minutes and
killed — while a byte walk over the same text took **0.88 s**. The difference is
not the reading. It is `pieces @ pieces.push(one)`.

**Everything below was run in the session that writes this file.**

## The curve

One program per size, `xs: [i64] @ []` then `xs @ xs.push(i)` in a `while`, timed
end to end (each includes ~0.5 s of compiling the program itself):

| pushes | wall | work | ratio to the row above |
|---|---|---|---|
| 25,000 | 1.37 s | ~0.9 s | — |
| 50,000 | 3.54 s | ~3.0 s | **3.3×** |
| 100,000 | 12.20 s | ~11.7 s | **3.9×** |

Doubling the count roughly **quadruples** the time. Building an array of n
elements is O(n²).

## The cause, in eleven lines of C

`runtime/parts/array.c:110`:

```c
HeroArrayHeader *hero_array_push(const HeroArrayHeader *a, const void *elem) {
    hero_array_require(a);
    if (a->len == INT64_MAX) hero_panic("array length overflow");
    HeroArrayHeader *b = hero_array_new(a->elem, a->len + 1);
    ...
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->copy(dst + (size_t)i * size, src + (size_t)i * size);
    }
```

Every push allocates a **new array of exactly `len + 1`** and copies every
element through `elem->copy` — an indirect call per element, per push. There is
no capacity, no geometric growth, and no fast path for an array whose refcount is
1. The emitted C is `t8 = hero_array_push(t6, &t7);` followed by the usual
incref/decref pair, so the old header is released immediately after: the copy is
paid and then thrown away.

## Why it matters beyond one scanner

`xs @ xs.push(v)` is how a Heroes program grows anything. `grep -c` over
`selfhost/` is not in this file because the count was not run; what *was* run is
the shape of the compiler's own cost, and it is superlinear:

| program | source | emitted C | `--emit-c` |
|---|---|---|---|
| `tests/harness/main.hero` | 3,630 lines | 124,149 lines | **22.0 s** |
| `selfhost/main.hero` | 34,812 lines | 724,245 lines | **1,011 s** |

A 9.6× bigger source costs **46× more time** (exponent ≈ 1.66). That is not proof
that push is the whole reason — nothing here attributes the 46× to a single
cause — but it is the shape a quadratic inner loop produces, and it is the only
quadratic this measurement found.

Today's other numbers, same machine, same day: `heroes check selfhost/main.hero`
**8m03s**, `heroes test selfhost/main.hero` **20m35s**, the seed build 3.4 s (that
one is clang, not this compiler).

## What a repair would have to answer

Not written here as a proposal, because it is a **panel path**: the fix touches
the runtime ABI and the emitter's ownership contract (CLAUDE.md §4 — architecture),
and it is queued in `docs/debrief/DECIDE.md`.

The question it has to answer: `xs.push(v)` is a **value** — `ys = xs.push(1)`
must leave `xs` untouched (spec:73, "no aliasing exists anywhere"). Growing in
place is therefore sound only where the old array provably dies, which is exactly
the shape `xs @ xs.push(v)` has and the shape `ys = xs.push(v)` has not. So the
knowledge lives in the **emitter**, not in the runtime: the runtime cannot tell
the two apart from a pointer. Panel 022's rule ("one unshare per step of a mutated
place") is the same idea one artifact earlier.

Three costs a repair must price, all of them the panel's to weigh: a capacity
field is an `HERO_RUNTIME_ABI` bump and a seed regeneration; a second entry point
(`hero_array_push_owned`) is emitter surface and a new way for the emitter to be
wrong; and any in-place path has to survive `--sanitize` plus
`hero_runtime_check_leaks()`, which is where an aliasing mistake shows up as a
use-after-free rather than as a wrong answer.

## The instrument this leaves behind

None yet, and that is deliberate — a benchmark asserted at a wall-clock threshold
is a test that fails on a busy machine. What exists is this file and the three
rows above: the next measurement of the same three programs says whether anything
moved.
