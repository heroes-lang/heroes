# Panel 088 — The sitting that had already been held

**Convened** 2026-08-23, on the author's word (*"per me puoi convocarlo anche subito
il panel"*) within the hour the finding was measured.
**Trigger** architecture — the runtime's mutation primitives and, once the spec
was read properly, `spec/heroes-spec.md:167-169`. CLAUDE.md §4.
**Status** `provisional — author ratification pending`.
**Seats** five. The last two were convened late by a coordinator error; see
§ Process notes, which is longer than usual because three of the errors are the
coordinator's.

**Frozen checkout**: `/tmp/heroes-panel-088`, a detached worktree at `9cddb83`,
because the main tree had an unrelated repair in flight. That answers the author's
own question — *"come facciamo con le modifiche pending?"* — and it half worked:
see § Process notes, error 3.

## The proposal, verbatim

> `hero_array_push` is O(n) per call, so growing an array of n elements is O(n²).
> Make it amortized O(1) without breaking value semantics.
> **O1** capacity field + in-place when refcount is 1 · **O2** a second entry point
> the emitter picks on the syntactic shape · **O3** geometric growth only ·
> **O4** refuse and document · **O5** something nobody listed.

## THE FINDING THAT SHOULD HAVE COME FIRST

**Panel 037, twelve days earlier, is titled *"Array growth, and the optimisation
that fires nowhere and corrupts where it fires"*.** It was ratified 2026-08-12. Its
resolution item 1 refuses O1 with two compiled vetoes. Its item 2 **adopts the
place-store form as the shape** and makes it wait, naming its own landing
conditions. `design.md:1531-1537` carries the same ruling in prose, including the
mechanism:

> "What does **not** work is making `push` append in place when the refcount is 1:
> panel 037 implemented it and the gate never fires (the ownership pass's own slot
> makes the count 2 at every accumulator push) … The sound form is a **place
> store** — `p @ push(p, v)` recognised at lowering, uniqueness taken from the
> place rather than guessed from a count — and it waits for M-selfhost-probe to
> measure whether anything needs it."

and the workaround with its numbers: chunked accumulation, **527,000 output lines
in 0.5 s against 403 s in one flat array**.

So O1 and O3 were settled, the reason was written down, and the sound form was
named — before this sitting opened. CLAUDE.md §1 names this failure by its shape
(*"a silence read as an open question"*) and prescribes the grep that catches it;
the grep was not run. **Panel 084 of this project is titled *the question that
design.md had already answered*. This is the second, and the first one whose brief
sent five judges after it.**

**What the sitting therefore is.** Not a discovery. Two things instead, and both
were owed:

1. **The measurement panel 037 deferred and M-selfhost-probe never took.** 037
   item 2 says *"M-selfhost-probe measures it"*; that milestone closed 2026-08-15
   with no such measurement. Four seats took it today.
2. **A working implementation of the adopted shape**, compiled and run — which
   nobody had.

## Measured in this sitting

Everything here was run on 2026-08-23. Attribution is marked where it is
inference, because the headline number turned out to be one.

### The cost, and the thing it is not

| what | number | by |
|---|---|---|
| 25k / 50k / 100k pushes | 0.9 s / 3.0 s / 11.7 s of work — 4× for a 2× | coordinator |
| the same at `-O2` | 1.52 / 5.09 / 18.33 s (≈3.5× per doubling) | spec-warden |
| `hero_array_push` | new array of `len+1` per call, `elem->copy` per element | `array.c:110` |
| 100k **map** inserts (`m[i] @ i`) | **0.75 s** — a hash map used as a list is **15×** faster than the language's own sequence | coordinator |
| `heroes lex grammar_expr.hero` (12,349 tokens) | 4.96 s | spec-warden |
| `heroes lex ir_lower.hero` (18,845 tokens) | **84.16 s** — 1.53× the tokens, 17× the time | spec-warden |
| **99% of push's copy traffic removed** (1,796,724,220 → 18,398,012 elements) | harness emit **12.29 s → 11.46 s**, **1.07×** | compiler-engineer |

**The last row is the sitting's most important number and it kills the headline.**
`docs/measurements/011` measured the compiler's emission scaling superlinearly —
3,630 source lines in 22.0 s against 34,812 in 1,011 s, 9.6× the source for 46× the
time — and explicitly did not attribute it. It is now measured as **not push**:
deleting essentially all of push's element copying buys 7%. **Where the 46× lives
is an open question, and it is the best thing this sitting produced.**

### The gate cannot be made to work, and this is now measured twice

- Instrumented `hero_array_push`, canonical accumulator: `pushes=100000 rc1=0
  rc2=100000`. **The rc==1 gate fires zero times** (ffi-pragmatist).
- Compiler-wide: rc2 is **94.2%** of 9,630,640 pushes; over `heroes check
  tests/harness/main.hero` the rc==1 path reaches **14.4%** of the copy work
  (186,189,031 of 1,294,923,330 elements).
- **`ys = xs.push(2)` and `xs @ xs.push(2)` both report rc=2** — the counter cannot
  tell the legal case from the illegal one, because it counts **slots, not
  liveness**: the borrow takes no reference (`ir_own.hero:14-16`, rule 1) and the
  second reference is the ownership pass's own slot, which exists by design
  (rule 5, `:26-40`).
- Loosening to `rc <= 2` **corrupts silently**: `xs=11 ys=11 zs=12` with
  `zs[10]=99` where 77 was written, **exit 0, clean under ASan+UBSan**
  (compiler-engineer). That is the worst possible failure mode in this project's
  own terms, and it is exactly what panel 037's title says.
- **O3 alone is inside the noise**, measured independently by two seats: 22.34/20.75
  s against 23.57/20.14 s (ffi-pragmatist), and 17.08 s against 16.22 s
  (compiler-engineer) — the second one *slower*. The cost is the copies, not the
  allocations.

### The sound form, built and run

`hero_array_push_owned(HeroArrayHeader **slot, const void *elem)` — 58 lines in
`runtime/parts/cow.c` plus 11 in the header, `HERO_RUNTIME_ABI` unchanged at 14.
**100k pushes 22.34 s → 0.00 s; 1M pushes 0.03 s.** Seven adversarial shapes
byte-identical to baseline and clean under `-fsanitize=address,undefined` with
`hero_runtime_check_leaks()`: empty, one-element, `[str]` × 20,000 with an alias
kept, `[[i64]]`, a record field place, a map value, and a `cstr` held by C across
2,000 pushes. Nine of ten examples matched their `.expected`.

The whole repair, and the guard is the third path:

```c
void hero_array_push_owned(HeroArrayHeader **slot, const void *elem) {
    HeroArrayHeader *a = *slot;
    ...
    if (a->refcount == 1 && a->len < a->cap) {
        int64_t before = a->refcount;
        unsigned char *place = hero_array_data(a) + (size_t)a->len * size;
        a->elem->copy(place, elem);
        if (a->refcount == before) { a->len += 1; return; }
        a->elem->drop(place);          /* the value reached this array — undo */
    }
    /* geometric copy, the new element LAST and read from the old block,
       which is still alive: `elem` may point inside `a`. */
```

**Two seats built it and only one survived the adjacent shape.** The
compiler-engineer's prototype passed nine shapes, emitted byte-identical C and was
clean under both sanitizers — then **exited 138 on `n.kids @ n.kids.push(n)`**. The
version above returns `len=2 inner=1` at exit 0, because its guard re-reads the
refcount **across** `elem->copy`. Delete the guard and the same program becomes
`SELF REFERENCE` and a panic at 134 — so the guard has the test that makes it fire
(§9).

### Two corrections to the brief, both measured

- **`HeroArrayHeader` already has `cap`** (`heroes_runtime.h:273`, written at
  `array.c:58`, read by nothing for arrays). O1 needs **no layout change, no ABI
  bump and no seed regeneration** — `docs/measurements/011` said the opposite and
  is corrected there.
- **A syntactic match on the shape is wrong 28% of the time**: the engineer's loose
  matcher took 365 sites, of which **101 were `b = a.push(v)`** — genuine copies —
  and the compiler died with *"read of an unassigned array slot"*. The
  coordinator's own counts are two different questions and both are reported:
  **517** lines are `x @ x.push(…)` with a bare local, **687** if a field path
  counts, out of **699** `.push(` lines in `selfhost/`. Neither is a licence: the
  place, not the spelling, is what decides.

### The spec is not silent, and the fear was inverted

`spec:167-169` teaches the cost — *"accumulating either in a loop is quadratic"* —
so §12 does **not** make this a compiler bug, and repairing it retires a spec
sentence. Ten wordings measured by editing the spec and re-running `heroes
measure` (baseline 3512, ceiling 4096):

| wording | max | delta |
|---|---|---|
| delete the paragraph | 3453 | **−59** |
| scope it to `str` only | 3493 | −19 |
| keep it, add an amortized clause | 3522 | +10 |
| **true, tight, keeps the example** | **3508** | **−4** |
| the same, keeping *"push returns a copy"* verbatim | 3518 | +6 |

**The sentence that is true after the repair is cheaper than the false one.**

- **Leaving the spec false is inadmissible**: it breaks §12, and the line has a
  **dependant** — `selfhost/measure_bpe.hero:51`, inside the tool that scores §1.6.
  `suite_spec.hero` locks the token count, not the truth, so nothing fires when the
  claim dies.
- **Deleting the paragraph is refused too**: the `str` half is still true, and
  panels 043 and 054 already refused that deletion on their own measurements.

### The sentence is already doing harm

The ergonomist wrote three programs from the spec alone. The `str` half **earned
its keep** — it moved the seat from character concatenation to an index scan. The
`push` half **changed nothing, because its remedy consumes the pathology**: `join`
takes a `[str]` that can only be built with `push`, `repeat` is str-only
(`error[bad_operand]`, measured), and while `xs[i] @ v` on an array **does** work,
an array cannot be preallocated — so the two-pass escape does not exist.

**And the warning steered the seat to a worse program.** Told arrays are quadratic
and told nothing about maps, it wrote a **map keyed by a counter, used as a list**.
It compiles, it loses ordering (`keys` is unordered, so every later traversal needs
`sort` and one that forgets is wrong at exit 0) — and it is **15× faster**. The
spec's silence about `{K: V}` is doing work nobody asked it to do.

Its trap for the *new* wording: drop *"push returns a copy"* and a reader learns
*"push is cheap"*, at which point `ys = xs.push(4)` looks cheap and costs a full
copy — the seat wrote the 10⁸-copy program, exit 0, no diagnostic. Its veto
condition is a constraint on the implementation: **the cheapness must be keyed to
the syntactic shape, not to runtime aliasing**, or a line's cost depends on lines
the reader cannot see. That is the same conclusion the ffi-pragmatist reached from
the C and design.md reached twelve days earlier.

### Precedent (advisory)

No value-semantics language got this from the runtime alone. **Swift** needed
SE-0176 exclusivity and `_modify` accessors — and a 2019 property-wrapper bug
silently restored the quadratic, fixed in the *compiler*. **Lean 4** and **Koka**
use borrow analysis. **CPython** clears the caller's slot before appending, and
that trick has had an **open bug since 2016** (non-atomic window). **Erlang**
ships binaries with a documented list of cliffs. Failure shapes on the record:
stale derived state (CPython's utf-8 cache, 2015), the non-atomic window, and a
callback freeing the buffer mid-sort (php-src #22678, 2026-07-10 — where the fix
reached three call sites and missed the fourth for twenty months). Growth factors:
doubling in Rust, libc++ and Swift; folly picked 1.5 with the only documented
reason — a doubling vector can never reuse its own freed memory. **The option
nobody listed**: RRB-trees — amortized-enough, no uniqueness check, no cliff, at
the price of contiguity, and if `[T]` never reaches C that price may be zero.

## The resolution adopted — the most conservative one

**R1. Nothing lands in the runtime or the emitter today, and O1/O3/O4 are closed
rather than deferred.** O1 fires on 0 of 100,000 accumulator pushes and corrupts
silently when loosened; O3 is inside the noise, measured by two seats; O4 is
**vetoed** by the ffi-pragmatist on §1.11 — bulk data from C reaches Heroes only as
`str → [str]` through `pieces @ pieces.push(…)`, so refusing taxes every
multi-result binding, and the veto lifts the day a C-owned buffer whose length
Heroes knows exists (Part 6 wart 17's own return condition).

**R2. The place store stays adopted and stays waiting** — this sitting changes
panel 037 item 2 in exactly one way: it supplies the measurement 037 asked for, and
the measurement says **not yet**. The compiler self-hosts; 1,011 s *finishes*;
removing 99% of push's copying buys **1.07×**. 037's landing conditions are
unchanged and one of them is the author's alone: *"or the author declares a stage
unacceptable"*.

**R3. The spec is untouched today, because it is still true**, and the amendment is
**pre-registered** for the commit that lands the place store: the −4 wording, which
keeps *"push returns a copy"*, names the shape, and makes the cheapness derivable
rather than memorised. Leaving it false was refused; deleting it was refused.

**R4. Five conditions ride with the place store**, all of them from seats that
compiled something:
1. the signature is `**` — uniqueness taken from the **place**, never from a count,
   never from a caller's promise (ffi-pragmatist; hard, and it switches to *object*
   otherwise);
2. the **self-reference guard** stays, with `n.kids @ n.kids.push(n)` as its firing
   test — one prototype died at exit 138 without it;
3. `Place.path.len == 0` only, or a **measured** argument for field paths
   (compiler-engineer) — 687 vs 517 is precisely that unmeasured gap;
4. an `ir_verify` check with a test that makes it fire, plus
   `a_push_owned_never_appends_to_a_shared_array` over `heroes mutate`'s corpus;
5. the spec amended in the **same commit** (R3), and panel 037 item 2's
   `HERO_RUNTIME_ABI` bump either honoured or **overturned in writing** — today's
   argument is that a missing function is a link error, louder than a stamp, and
   that argument deviates from a ratified item.

**R5. The 46× becomes its own question, and it is the sitting's real product.**
Emission scales as 9.6× source → 46× time, exponent ≈1.66, and push is **7%** of
it. Nothing in this sitting knows where the rest is. Queued in `DECIDE.md` with the
three candidates nobody has measured: the `check` phase (8m03s alone), the
interner/type-table walks, and the writer's own string accumulation.

**R6. Four record repairs**, three of them found on the way:
- `docs/measurements/011`'s two false cost sentences — **corrected in place**, with
  the panel's numbers;
- `runtime/heroes_runtime.h:19` says *"HERO_RUNTIME_ABI is 3"* over `#define … 14`;
- panel 037 item 2 names `c4.hero` and `c5.hero` as required cases and **neither
  exists anywhere** — verified by two independent greps;
- panel 037 item 2 assigned its measurement to M-selfhost-probe, which closed
  2026-08-15 without taking it. Twelve days, and the deferral was invisible from
  both ends.

## Predictions to score

| # | seat | prediction | scored at |
|---|---|---|---|
| 1 | compiler-engineer | with the place store landed, `heroes build tests/harness/main.hero --emit-c` stays **above 9.8 s** against a 12.29 s baseline (speedup under 1.30×) | the milestone that lands it |
| 2 | compiler-engineer | that milestone's diff over `selfhost/` + `tests/` exceeds **200 lines**, breaching panel 037's own ≤200 condition | the same |
| 3 | spec-warden | the landed wording measures **3508** (`SPEC_TOKENS 3512 → 3508`, a new row in measurement 010); falsified at ≥3512 | the same commit |
| 4 | spec-warden | `heroes lex selfhost/ir_lower.hero` drops from **84.16 s to under 10 s**; falsified above 40 s, and then the repair entered on comfort | the same |
| 5 | ffi-pragmatist | O1 alone can never remove more than **14.4%** of the compiler's array copying, and none of the accumulator's | any future O1 attempt |
| 6 | ffi-pragmatist | §4.19's ladder needs no shim and no ABI bump under any option — `grep -c '\.push('` over the four real bindings stays `0 0 0 0` | M-ffi-ladder |
| 7 | llm-ergonomist | under the current sentence, ≥3/20 generated programs avoid `push` by restructuring (counter-keyed map, or folding away the list); under the −4 wording, 0/20 | when a metric-2 arm exists |
| 8 | llm-ergonomist | under a wording that drops *"push returns a copy"*, ≥2/20 call `push` on a receiver read again afterwards inside a loop — silently quadratic at exit 0 | the same |
| 9 | historian | the first thing the repair breaks is a primitive that borrows the buffer across a user function value — `sort`, or `map`/`filter`/`fold` | the milestone that lands it |

## Process notes — three of the errors are the coordinator's

**1. The sitting should never have been convened in this shape.** One `grep` over
`design.md` and `docs/panel/` for `push`/`refcount`/`in place` would have found
panel 037 and §4.10's paragraph. CLAUDE.md §1 prescribes exactly that grep, by
name, for exactly this failure. The cost: five seats and roughly three hours of
machine time to re-derive a ruling that was twelve days old. What partly redeems it
is R5 and the prototype — but neither was the plan.

**2. The blind A/B was not blind.** The ergonomist's brief quoted the sentence
verbatim, called it *"the sentence the sitting is about"*, and then asked the seat
to judge two variants — so it knew which was current and said so in its own first
paragraph. The substantive half of its report stands; the blind half does not, and
the seat's own instruction for next time is on the record: brief the variants with
no pointer to the line.

**3. The frozen checkout was shared, and it was therefore not frozen.** One seat
instrumented `runtime/parts/array.c` inside the worktree while another was timing
against it; the second seat's first baseline was contaminated and had to be
re-measured against `git archive HEAD`. Panel 056's freeze rule was applied to the
coordinator's own edits and not to the judges' — **one worktree per compiling
seat** is the rule that was missing, and it is cheap: `git worktree add` is seconds.

**4. The lane was chosen on a false premise** — *"this change spends no spec
token"* — when `spec:167-169` teaches the very behaviour. The historian found it,
the warden priced it at **−4**, and the cost of the wrong lane was not tokens: it
was that *"leave the spec false"* reached the ballot at all.

## Author's verdict

*(pending — `docs/debrief/DECIDE.md`)*
