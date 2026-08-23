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

## Corrections — 2026-08-23, hours later, from panel 088

**Two sentences above are wrong and the record says so rather than being
rewritten.** The ffi-pragmatist measured both; the coordinator verified both by
hand before writing this.

1. **"A capacity field is an `HERO_RUNTIME_ABI` bump and a seed regeneration" is
   false — the field already exists.** `runtime/heroes_runtime.h:273` declares
   `int64_t cap;` in `HeroArrayHeader` and `runtime/parts/array.c:58` writes it.
   Nothing ever reads it for an array: `grep -rn -- "->cap" runtime/` answers only
   `array.c:58` (the write) and `map.c`/`map-write.c` (a different struct). So an
   in-place path changes no layout, needs no ABI bump and needs no new seed.
2. **This file convened a panel on a question `design.md` had already answered,
   and that is the finding worth keeping.** `design.md:1531-1535` says, in so many
   words: *"What does **not** work is making `push` append in place when the
   refcount is 1: panel 037 implemented it and the gate never fires (the ownership
   pass's own slot makes the count 2 at every accumulator push) … The sound form is
   a **place store** — `p @ push(p, v)` recognised at lowering, uniqueness taken
   from the place rather than guessed from a count — and it waits for
   M-selfhost-probe to measure whether anything needs it."* It also records the
   workaround with its numbers: accumulate in chunks, **527,000 output lines in
   0.5 s against 403 s in one flat array**. CLAUDE.md §1 names this exact failure
   ("a silence read as an open question") and prescribes the grep that would have
   caught it. It was not run.

**What the sitting therefore is**: not a discovery, but the measurement
`design.md` was waiting for — M-selfhost-probe closed on 2026-08-15 without taking
it — plus a working prototype of the form `design.md` already named. The gate's
failure is now measured rather than remembered: an instrumented
`hero_array_push` reports `pushes=100000 rc1=0 rc2=100000` on the canonical
accumulator, and over `heroes check tests/harness/main.hero` the rc==1 path
reaches **14.4%** of the copy work (186,189,031 of 1,294,923,330 elements).

A third correction, found on the way and belonging to another file: the prose at
`runtime/heroes_runtime.h:19` still reads *"HERO_RUNTIME_ABI is 3"* over
`#define HERO_RUNTIME_ABI 14` at line 31.

## The instrument this leaves behind

None yet, and that is deliberate — a benchmark asserted at a wall-clock threshold
is a test that fails on a busy machine. What exists is this file and the three
rows above: the next measurement of the same three programs says whether anything
moved.
