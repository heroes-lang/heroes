# Panel 037 — Array growth, and the optimisation that fires nowhere and corrupts where it fires

**Convened** 2026-08-12, by author instruction after `/debrief doubts` question 1.
**Trigger** architecture (the runtime's mutation primitives) — CLAUDE.md §4.
**Status** `provisional — author ratification pending`.

## Why it was convened at all

CLAUDE.md §13 makes performance **a non-goal, never a justification**, and panel
028 had already recorded this measurement with the note *"Part 2 forbids using
this as an argument"*. The author re-read the rule the same day:

> *"le prestazioni non sono un goal ma non devono essere nemmeno un limite"* —
> performance is never a **reason**, and slowness is not acceptable as a
> **ceiling** where it stops a closure-list program from running.

So the question put to the judges was not *is this faster*. It was: **does the
self-hosted compiler run at all without it**, and **is the mechanism sound**.

## The proposal, verbatim

> `hero_array_push` appends **in place** when the array is uniquely owned and has
> spare capacity, and grows `cap` **geometrically** when it does not. When the
> array is shared, it copies as it does today.

Today (`runtime/parts/array.c:93-109`) every push allocates a fresh array of
`len+1` and deep-copies every element through the descriptor. Measured
2026-08-12: **50 000 pushes 2.0 s, 100 000 pushes 7.8 s** — twice the size, four
times the time.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| ffi-pragmatist | **veto**, unsoundness | built three runtimes. The proposal read literally **runs out of memory** (a shared array doubles `cap` once per element); fixed, it leaves the curve unchanged; and it silently corrupts a nested array with **every instrument reporting success** |
| compiler-engineer | **veto** | implemented it verbatim: **the in-place gate never fires**. At every `out @ push(out, i)` the refcount is **2** — `own.rs` rule 5's synthetic slot holds one and the named slot the other |
| spec-warden | **object** · 0 tokens | the refusal was never a Part 2 refusal: `heroes_runtime.h` and `emit/aggregate.rs` both give **§4.10 soundness** as the binding reason. The author's re-reading unlocks a footnote and leaves the ratio standing |
| llm-ergonomist | **approve** — of a different proposal | reading only the spec: *"the document does not fail to state the cost; it states semantics whose literal reading gives the quadratic answer, and I overrode it on a hunch"* |
| historian (advisory) | **approve** with an instrumentation condition | four refcounted value-semantics runtimes ship this check. Their recorded failure mode is not corruption but a **silent cliff** — Erlang documents it, PEP 8 tells you not to rely on CPython's |

## The proposal fires nowhere, and two judges found it independently

This is the session's result and it took one implementation to get.

**The gate never opens.** `own.rs` rule 5 gives a call's result a synthetic owning
slot, and the named slot increfs — so at the moment `hero_array_push` is entered,
the array it is asked to append to has refcount **2**, not 1. Measured by the
compiler-engineer end-to-end (9.27 s vs 8.33 s at n=100 000) and by the
ffi-pragmatist with an instrumented runtime:

```
inplace=0  grown=0  shared=50000    rc: 1=0  2=50000
```

Fifty thousand pushes out of fifty thousand take the copying path. `range`, `map`,
`filter` and `args` — every Tier-2 accumulator in `library/source.hero` — are this
shape.

**And where it does open, it is wrong.** An array held only by an enclosing
array's element slot has refcount 1 and a live observer:

```
baseline: 67 / false        patched: 77 / true      # grid[0] grew behind grid's back
```

Exit 0. `--sanitize` silent. `hero_runtime_check_leaks()` green. **All 68 golden
`run` and gallery programs byte-identical.** It is panel 022's shape one primitive
over: the corpus cannot see it, and spec line 62 — *"No aliasing exists
anywhere"* — becomes false with no diagnostic.

Relaxing the test to `rc <= 2` makes it fast **and** makes `b = push(a, x)` alias
`a`. Both judges checked; both refused.

## The answer both vetoing judges converged on, compiled

Not a better refcount test — **no refcount test**. `hero_array_push` keeps its
value semantics and its signature untouched, and the *place store* every other
mutation primitive already has is added beside it:

```c
void hero_array_append(HeroArrayHeader **slot, const void *elem);
```

Uniqueness comes from `hero_array_unshare(slot)`, not from a guess. It is
recognised at lowering only where the statement is `p @ push(p, v)` with `p` a
whole slot — **a fact about that instruction**, destination place and first
argument being the same place, rather than a premise about the world (CLAUDE.md
§11). It is the shape `hero_map_set` already has.

| | today | place store |
|---|---|---|
| 50 000 | 2.04 s | 0.000 s |
| 1 000 000 | **805.95 s** | **0.004 s** |
| `b = a; a @ push(a, 9)` | `len(a)=4 len(b)=3 distinct=yes` | identical |

ASan and UBSan clean, leak counter zero.

## It is not compiler-need, and that is measured too

Both judges took the ceiling test seriously and both report it does not fire.
Extrapolating from `examples/calculator.hero` (382 lines → 2 508 tokens → 8 056 C
lines), a ~25 000-line port builds a 164 000-element token array (**22 s**) and a
527 000-element output array (**403 s**). But **chunked accumulation compiles in
Heroes today** — accumulate per declaration, flush into `[[str]]`, `join` at the
end — and measures **0.72 s**. The workaround does not merely compile: it
finishes.

M-selfhost-probe's own rule is *"a form whose workaround compiles is a Part 7
deferral by default"*, and the warden supplied the substitution this case needs:
not *does the workaround compile* but **does the program finish**. It does.

## The finding neither half of the brief contained

Both the compiler-engineer and the spec-warden arrived at it independently, from
opposite directions:

> **`join` was funded as the remedy for O(n²) string building, and you cannot
> reach `join` without an O(n²) array build.**

design.md §4.10 says *"on a hundred thousand lines of generated C it becomes real
waiting. Provide `join([str]) -> str` or a small `Builder`."* `join` shipped.
Nothing in the document notices that `join`'s argument is itself built with
`push`, so its own prescribed remedy is defeated by the container it is handed.
Part 8 wart 8 repeats the incomplete claim. **That is a documentation defect, not
a proposal, and it is corrected in this commit.**

## The llm-ergonomist answered a different question, and its answer stands alone

Given only the spec, it wrote three programs and reported that it assumed `push`
was cheap **from the absence of an escape hatch**, not from the text — and that
the text, read literally, says the opposite:

> *"Mutable parameters are marked `@` … Semantics: copy in, copy out."* If `push`
> takes `@xs: [T]`, each push copies the array in and copies it back out. n pushes
> is n².

It is the only silent hesitation it found: every other guess it made failed
loudly. Its remedy is one clause naming `join`, and it **pre-vetoed** the obvious
wording — any phrasing that explains copy-on-write makes the cost of a line
depend on whether some other binding is live fifty lines away, which is a fact
that expires silently.

This is a spec question, separable from the runtime one, and it is queued with
its own measurement rather than adopted here: the warden measured the *runtime*
change at **0 tokens** and never measured the clause.

## The resolution — provisional, author ratification pending

1. **The proposal is refused.** Two vetoes, both compiled, and the milder finding
   is decisive on its own: it fires nowhere.
2. **The place-store form is adopted as the shape**, and it **waits**. Soundness
   without need still waits (spec-warden); the port finishes with chunked
   accumulation at 0.72 s. **M-selfhost-probe measures it**, and it lands there if
   a closure-list stage does not finish or the author declares a stage
   unacceptable — with `c4.hero` and `c5.hero` as named cases (CLAUDE.md §9) and
   `HERO_RUNTIME_ABI` bumped.
3. **design.md §4.10's `join` gap is corrected now**, because it is a false
   sentence rather than a missing feature.
4. **The spec clause is queued, not adopted**, with the ergonomist's veto on
   copy-on-write phrasing attached to it.
5. **`emit/aggregate.rs` throws away a literal's capacity on the first push** — a
   2-line fix, worth nothing on short literals, and it travels with item 2.

**What a veto would compel.** If the author overturns item 2 toward landing the
place store now, it is an IR and ownership change (a new `Op`, six exhaustive
`match` sites) and CLAUDE.md §4 makes that its own panel — which is what the
spec-warden's condition 1 says in the other direction.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| ffi-pragmatist | if the proposal ships as written, `library-range.hero` reports **`inplace=0`** and 100 000 pushes still take **7.8 ± 0.4 s** | now — already observed |
| compiler-engineer | the probe's ported lexer finishes any single module in **under 0.2 s** and reports **no** `push` blockage: no corpus file exceeds ~2 600 tokens, so the cost is invisible below ~50 000 elements and the probe **structurally cannot see it** | M-selfhost-probe |
| spec-warden | the probe finds **zero** closure-list programs that fail to terminate because of `push`; a fixpoint stage exceeding 30 minutes with chunked accumulation withdraws it | M-selfhost-probe |
| llm-ergonomist | under the current spec **≥50%** of generations write `s @ s + piece` in a loop rather than `[str]` + `join`; the clause drops it **below 20%**, and the `push` half alone moves it **<10 points** | Part 11 harness |
| llm-ergonomist | on "first n primes", **<20%** divide by the collected primes now, **>60%** with the clause | Part 11 harness |
| historian | instrumented over the corpus, the copy count is **non-zero for at least one loop whose array is logically unique**, and the cause is a refcount raised by the ownership pass rather than by a genuine second value | already observed — 50 000 of 50 000 |

## Conditions on the record

- **ffi-pragmatist**: if the value-form rule is adopted anyway, it is a **hard
  veto until `const` leaves the signature** — a runtime that writes through a
  `const HeroArrayHeader *` while the header says it does not is a wrong FFI
  signature that is not a compile error, which is this project's thesis inverted.
  It also recorded that `HERO_RUNTIME_ABI` stayed at **10** across a
  semantics-only change and every generated unit accepted it: *the least-guarded
  change this ABI has*.
- **compiler-engineer**: the place-store form only, `Place.path.len == 0`, a
  `ir/verify.rs` check with a test that makes it fire, diff ≤200 lines, never
  touching `types/`.
- **spec-warden**: names three shapes it would veto, and one is worth quoting —
  *any resolution citing 2.0 s / 7.8 s as the reason*. Under the author's
  instruction those numbers are a trigger to run the ceiling test, never a
  justification. It ran the test; it does not fire.
- **historian**: the premise this rests on is *a slice never shares a buffer with
  its source*. `slice` is on the closure list; if it ever returns a second header
  over one allocation, D's stomping and V's issues #119 and #6569 become the
  expected failures. CLAUDE.md §11 wants that written as a test —
  `a_slice_never_shares_a_buffer_with_its_source`.

## What this session cost, and the process finding

Thirty minutes of wall clock, five judges, four runtimes built — for a question
two judges answered the same way. The author stopped it mid-flight and said so.

**The instrument has one gear.** `/panel` convenes five differentiated judges and
every one of them compiles, which is right for *changing the language* and absurd
for *is this optimisation sound*. The teaching process is amended by author
instruction (CLAUDE.md §4), and the amendment is in `/panel`: a **soundness
lane** — compiler-engineer and ffi-pragmatist only — for a proposal that changes
no surface, no diagnostic and no spec token. On this session it would have
returned both vetoes, the measurement and the place-store answer, and left the
other three judges out. The ergonomist's finding would have been lost, which is
the lane's stated cost: it is chosen when the question has no reader-facing half,
and this one did.
