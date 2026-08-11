# Panel 026 — the map is funded, and the surface the funding buys is not the one proposed

**Convened** 2026-08-10, on the author's verdict on panel 022 R4. **Trigger**
`spec/**` plus architecture (CLAUDE.md §4). **Status** the decision to fund is the
author's and retro-record; the *surface* is this panel's, and it is
`provisional — author ratification pending`.

## The decision

Panel 022 R4 left `{K: V}` open: fund `set(m, k, v)` plus `for k in m` at **+29**,
or delete the container for **−57**. **The author chose to fund it.**

The convener put the choice to the author with a reason, and **the reason was
wrong**. It said a compiler needs a symbol table built incrementally, so a map with
no insertion is unusable. True as far as it goes and inverted where it matters —
see R5. The verdict stands on other grounds, which the panel supplies.

## The candidates, measured jointly

| | | binding | Δ |
|---|---|---|---|
| committed spec | — | 2304 | — |
| `m-keys` | `set(m,k,v)` + `for k in m` (keys) | 2340 | +36 |
| `m-pairs` | `set(m,k,v)` + `for k, v in m` | 2333 | +29 |
| `m-place` | `m[k] @ v` + `keys(m)` | 2351 | +47 |
| **`m-place` − `has`** | **the resolution** | **2347** | **+43** |

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **veto**, scoped to `set(m, k, v)` as a void call | measured that the shipped map cannot yield *any* content-determined order — so **design.md:1147 and ROADMAP:319 are already false** |
| llm-ergonomist | approve `m-pairs`, object to the others | with the committed spec it **abandoned `{K: V}` entirely** and wrote O(n²) arrays: *"the map of `m-a` is decorative — a type with a reader, a membership test, no writer and no enumeration"* |
| spec-warden | approve only `m-pairs − has`; **scored its own prediction falsified** | panel 022's +29 is **refuted, not confirmed**; and the symbol-table argument is backwards |

## Where they converged, from opposite ends

**`set(m, k, v)` is the wrong surface, and two judges said so for unrelated
reasons.**

The engineer, reading the tree: a void `set` needs `Arg::InOut` on a
`Callee::Builtin`, which exists nowhere today; `aggregate::place` extended past its
`Step::Index(_) => return text` bail-out; and §4.8's copy-out — which lives as
`Op::CopyOut` **in the callee's body** and is asserted per exit edge by
`ir/verify.rs` — hung on a callee that **has no body**. Checker, ownership pass and
emitter each grow a special case: **core, by §1.7's own test.**

The ergonomist, reading only the spec: the builtin list writes `set(m, k, v)`
**without `@`**, while §4.8 says a mutable parameter is marked at the call site. So
read literally `set` does not mutate and must return a new map — and if it does
mutate, `set(counts, w, v)` as a statement **compiles and silently does nothing**,
leaving the map empty and the program printing nothing. *"A plausible mistake that
compiles. This is the enemy."*

**`m[k] @ v` costs no IR change at all.** `ir/places.rs:69-74` already pushes
`Step::Index(key)` for any indexable base; `Step::Index` carries a `ValueId`, so a
`str` key fits unchanged; `ir/uses.rs:79` already counts it as a use; and
`own.rs`'s `indexed` rule — incref before the store, no decref, because the
primitive drops the old value and moves the new one in — **is already exactly
`hero_map_set`'s required contract.**

## The measurement that makes a shipped claim false

The engineer compiled a probe against the real runtime:

```
3 pairs   cap=8    len=3   iterates  b a c
4 pairs   cap=16   len=3   iterates  b c a      hero_map_eq(a, b) = true
```

`cap` is derived from the literal's pair count **including duplicates**
(`emit/aggregate.rs:444` passes `arguments.len() / 2`), so **two `==`-equal maps
written differently iterate differently**. Insertion order is not merely
unavailable at iteration time — `HeroMapHeader` has no order field, so it is **not
stored**, and no iteration strategy can recover it. Sorting cannot either.

Therefore:

- **design.md:1147** — *"Map iteration follows insertion order… Overwriting an
  existing key keeps its position"* — is false as implemented.
- **ROADMAP:319** — *"iteration order must be a function of the contents"* — is
  false as implemented.

Under CLAUDE.md §12 that is the compiler's bug, not the design's. But the
resolution below removes the promise instead of paying for it, and the reason is
that nothing in the language will observe it.

## Resolution — provisional, author ratification pending

### R1 · `{K: V}` stays, funded. The author's verdict, recorded.

### R2 · The mutating surface is `m[k] @ v`, not `set(m, k, v)`

The engineer's veto is scoped and it lifts on exactly this. `m[k] @ v` is a
`Store` with a trailing `Step::Index`, which the IR already builds. The one new
checker rule is the **write-side asymmetry** — `m[k]` *reads* `V?` and *writes* `V`
— plus a rule confining a map step to the **last** step of a place. And the
divergence from arrays must be named in the spec, because it is real: `xs[i] @ v`
aborts when the index is absent; **`m[k] @ v` inserts.**

### R3 · Iteration is `keys(m) -> [K]`, composed with the `for` and `sort` that exist

Not `for k in m`, not `for k, v in m`. The engineer's costing: pure sugar, **zero
new IR ops**, ~14 lines of C and four table rows, and it works with **either** data
structure — so it does not force the rewrite that R4 declines. CLAUDE.md §10's
stopping rule says the shape is **nothing** when two existing invocations compose
to it, and `for k in sort(keys(m))` is that composition.

It also serves the one place the bootstrap iterates a map — `types/holes.rs:163`,
whose own comment says it depends on **sorted** order. `for k in m` would not have
served it; `keys` plus `sort` does.

The ergonomist ranked `pairs > keys` and its argument survives into this shape
rather than against it: what it disliked about `keys` was the mandatory
`m[k].must()` in the loop body, an abort justified by the line above rather than by
itself. Under `keys(m)` the same lookup is written by the author who chose to write
it, in a loop they wrote, over an array — no clause of the spec obliges an unwrap
in a body.

### R4 · The order is **unspecified**, and the promise is deleted rather than bought

design.md:1147's insertion-order guarantee is struck. What replaces it is what the
implementation can honestly say and what the composition makes safe: **the order of
`keys(m)` is unspecified; a program that needs an order sorts.** The spec teaches
the composition at the point of use, which is §1.4's redundancy spent where the
error would occur.

This is the engineer's option 2c, and it costed 2c as *expensive in the thesis* —
because panel 006 measured that a model's spec-silent default **is** insertion
order, and Go's randomization is the recorded disaster of *unspecified* order. That
objection is answered by R3 rather than overruled: the hazard needed a reader who
iterates a map directly and assumes an order. With no `for k in m` in the language,
the only way to iterate is an array whose order the spec declines to promise, one
line above the `sort` that fixes it.

### R5 · The compiler-needs justification is withdrawn, and the record says why

The warden counted the 20 `.insert(` sites: **3** are `Vec::insert` in tests, **5**
are `BTreeSet` (**Heroes has no set type**), **6** are keyed by a dense arena index
(a side-table `[T]` serves), and of the 6 genuine maps one is keyed by `(u32, u32)`
(**Heroes has no tuples**), one by `Vec<TyId>`, one by a recursive `Ty`. **Exactly
one** is unambiguously `{str: int}`.

And the symbol table argues the other way: `resolve/scope.rs` is
`entries: Vec<(String, u32)>` with `marks`, chosen because a lexical scope needs
**shadowing** and **O(1) pop** — which `{K: V}` plus insertion provides neither of,
and the map has **no delete at all**. *"For a symbol table the map is not
unnecessary; it is the wrong structure."*

So `{K: V}` does **not** enter on §1.0's compiler-needs branch. It enters on the
thesis branch, where a *measured* effect is required and a forecast is not. What
would count, stated now so it cannot be softened later: the M6 harness, both arms,
on ≥2 tasks that naturally want a dictionary, measuring first-try compile-and-run
against an arm whose spec deletes `{K: V}`. At +43 net and 500–2000 tokens per
round trip, it pays for itself if it avoids **one rewrite per ~40 generations**.

The ergonomist's independent evidence is the strongest thing the funding has: given
the committed spec it **abandoned the map entirely** and wrote 31 lines of O(n²)
arrays where the funded spec writes 20.

### R6 · Nothing is owed for mutation during iteration

The warden priced a clause at +11 to close it. The engineer measured that it does
not need closing: `ir/control.rs:87-88` stores the sequence in an increfed
synthetic holder, so the refcount is ≥ 2 and a write inside the loop unshares —
**the loop already iterates a COW snapshot.** The clause is not written.

### R7 · `has` is removed, −17, and the warden's prediction is recovered rather than falsified

`has` is map-only (`types/builtins.rs:114`), so `has(m, k)` is exactly
`!m[k].is_err()` — two spellings of one predicate, and design.md:1146's stated
reason for it (*"its absence forced a sentinel-value hack"*) was retired by
design.md:1145 (*"map access returns `V?`, always"*).

The warden's panel-025 prediction was *"the next amendment is net ≤ +25 **or**
carries a measured removal"*. It scored itself falsified against the bare
candidates and named the recovery: with a removal, the disjunct holds. **+43 with
`has` removed carries one.** The prediction survives on the branch it named, and
the warden's refusal to carve out "funded author decisions" as a separate class is
recorded as the right call: *"CLAUDE.md §4 means no spec change lands without
author ratification, so a funded author decision is not a subclass of amendment,
it is the only class."*

### R8 · Panel 022's +29 is refuted, and a new rule comes out of it

Panel 022 priced the **keys** shape at +29 on base 2196. That wording measures
**+36** today. The +29 now attaches to `for k, v in m`, which panel 022 never
priced — *"the agreeing digits describe a different sentence on a different
base."* Fourth recorded instance.

The new rule the data supports: **removal deltas transfer across spec versions;
insertions must be re-measured.** `.is_err()` was −11 on base 2196 and is −11 on
base 2304, because deleting a table row is local while inserting into flowing prose
re-tokenises its neighbourhood.

## What a veto would have compelled

Had `set(m, k, v)` landed as proposed: `Arg::InOut` on a builtin, a copy-out with
no callee body to hold it, and a special case in the checker, the ownership pass
and the emitter — a construct that is core by §1.7's test, bought to spell an
operation the IR already expresses as a place store.

## Predictions to score

| # | judge | prediction | checkable |
|---|---|---|---|
| 1 | compiler-engineer | with iteration as `keys` over the existing `for`, `Op` keeps **exactly 21 variants** and the loop change is **≤20 Rust lines**; as a cursor op it touches **≥6 files** and `ir/control.rs` passes 320 lines | M6 close |
| 2 | compiler-engineer | `runtime/runtime.c` passes **1000 lines** with the map's mutation half — and panel 006's "≤500 with the map included" is **already false at 794**, so line estimates from that seat are discounted | M6 close |
| 3 | llm-ergonomist | counting/grouping first-try: committed spec **25–40%**, funded **70–85%**; and **≥60%** of committed-spec attempts invent a map-insertion form | next harness run |
| 4 | llm-ergonomist | `_` must be legal in a loop binding for a pair form to subsume a key form. **Verified legal** (`for _ in xs` runs), so the condition does not fire — recorded because it would have flipped its verdict | done |
| 5 | spec-warden | with `has` gone, `!m[k].is_err()` costs **0 ± 4** points against `has(m, k)`; and `set` shows **0 ± 4** on non-dictionary tasks | next harness run |
| 6 | spec-warden | the binding count stays **< 2450 through 2026-09-30** — 2347 leaves 103 | that date |

## Watch list

- **The growth alarm the warden filed, and it demands the delta gate.** The spec
  went 2231 → 2304 in one cycle: **+73**, against **+183 across the preceding
  eight** amendments. Its proposed gate: any single amendment above **+25 measured**
  requires a named measured removal, no exceptions, enforced by `heroes measure` in
  CI against the previous commit. This amendment complies by construction; the gate
  is queued.
- **Zero `// ORDER:` marks in the tree**, which design.md:1152 requires at every
  ordering-sensitive map walk in the bootstrap. Nothing today exercises the
  guarantee R4 deletes — which is part of why deleting it is cheap.
- **`emit/aggregate.rs` is 496 lines** and `emit/perfn.rs` 589; the map's place
  store lands in the first and forces a split (CLAUDE.md §11).
- **`cap` derives from the literal's duplicate-inclusive pair count.** Independent
  of the order question, that is a wasteful allocation and a fingerprint of the
  source text in the data structure. Worth its own look.

## DESIGN-LOG

Appended 2026-08-10 — see the lines citing panel 026.

## Ratification — 2026-08-12, by author instruction

**RATIFIED.** The author's instruction was a blanket one — *"ratifica anche tutto
quello che c'è da ratificare"* — given after reading the session summary, not a
clause-by-clause review of this file. It is recorded that way on purpose: this
project's own rule is that a record must not say more than what happened.

What it settles: the provisional resolution above **stands as the decision**, and
work no longer proceeds on it as a default. Every resolution here had been
load-bearing since the day it landed, so this changes the record's status rather
than the compiler's behaviour.

What it does **not** settle: anything this file keys to a measurement that has not
been taken. Those stay open on their own terms, listed in `docs/debrief/QUEUE.md`,
and a blanket yes cannot make a number arrive.
