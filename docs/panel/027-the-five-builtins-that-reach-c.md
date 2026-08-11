# Panel 027 — the five built-ins that reach C, and the field that would have restored a SEGV

**Convened** 2026-08-11, on M6 step 3. **Trigger** `spec/**` plus surface semantics
and a diagnostic class (CLAUDE.md §4). **Status** `provisional — author ratification
pending`.

## The proposal, verbatim

> M6 step 3 — the five built-ins that reach C. Three decisions: (1) `sort(xs)` on
> `[T]` — what total order, and for which T: add a `cmp` to the descriptor ABI
> (order for every type, lexicographic by field / case index) versus scoping `sort`
> to `[int]`/`[f64]`/`[str]` with a diagnostic naming the reason. (2) `slice` on
> `[T]` — out-of-range behaviour (abort like `hero_str_slice`, clamp, or `T?`), and
> whether panel 025 R2's held spec sentence now lands, since its stated trigger was
> "slice on [T] compiles and its abort is verified on both". (3) `chars(s)` on a
> `str` that is not valid UTF-8 — reachable today, because `slice` cuts bytes and
> can halve a multi-byte character: abort, or yield the raw byte as a one-byte
> `str`. Also in scope but not a language change: `join([str], str)`, `to_int(f64)`
> (spec already says out of range aborts), `to_f64(int)`.

Shorthand: **S-desc** is the sixth `HeroDesc` field; **S-scalar** is `sort` on
`int`, `f64`, `str` only.

## The candidates, measured before the judges saw them

Base **2334** (commit `717bc26`). Every number re-measured by the warden and
reproduced to the token; the convener re-measured them a third time.

| | sentence(s) | count | Δ |
|---|---|---|---|
| A | the spec as it stands | 2334 | +0 |
| B2 | "`sort` needs `int`, `f64` or `str` elements." | 2352 | +18 |
| C2 | "`slice` out of range aborts." | 2344 | +10 |
| E | B2 + C2 | 2361 | +27 |
| F | E + "`chars` aborts on invalid UTF-8." | 2373 | +39 |
| **W5** | line 129 rewritten: "An out-of-bounds **index or slice** aborts" | 2335 | **+1** |
| **W7** | W5 + "**and so does a slice that splits a character**" | **2345** | **+11** |
| W2 | the inventory parenthetical `sort` (`int`, `f64` or `str`) | 2345 | +11 |

W5/W7/W2 are the warden's own wordings, produced under objection to all of mine.

## The verdict table

| judge | (1) `sort` | (2) `slice` | (3) `chars` | its own finding |
|---|---|---|---|---|
| compiler-engineer | **veto S-desc**, approve S-scalar | approve abort | **object — wrong built-in** | S-desc does not serve the port either: the bootstrap's four sorts are `sort_by_key` over `span.start`, and a field-order `cmp` sorts `Diagnostic` by *kind* |
| ffi-pragmatist | **veto S-desc**, approve S-scalar | approve abort | **object — abort kills a real binding** | measured: a sixth field leaves all seven descriptors `cmp = NULL` **silently under this project's own flags**, and the call is `SEGV` with no type name |
| llm-ergonomist | approve the restriction | approve abort | approve abort *(overruled — see R4)* | under the silent spec its `chars` program prints **four confident lines, one of them garbage**, with no error: its worst finding |
| spec-warden | **object to any `sort` sentence** | approve the fact at **+1**, not +10 | **veto F** | F contradicts design.md:809, which puts the error at the *slice*; and the defect is real — `slice("caffè", from: 0, to: 5)` **exits 0** with a corrupt byte |
| historian (advisory) | approve the restriction | approve abort | approve moving the error | Go had to **fix** `slices.Sort` for NaN; Swift does **not** synthesise `Comparable` for structs; Python's silent clamp has **no documented body count** |

## Where they converged, from four different rooms

**S-desc dies twice, and neither seat could see the other's reason.**

The ffi-pragmatist compiled it. C11 6.7.9p21 zero-fills a short initialiser list, so
the *unchanged* `runtime.c` compiled clean against a six-field header under
`FLAGS` — no diagnostic — with `hero_desc_int.cmp == 0x0` and six others beside it.
`-Wextra` sees it, this project's flag set does not, and
`-Werror=missing-field-initializers` is **blind to the designated-initialiser form**,
so it would guard a code style rather than an invariant. The call is
`AddressSanitizer: SEGV (<unknown module>)`, *"can not provide additional info"* —
which is panel 022's null-`hash` argument, verbatim, one field over. It further
measured that `_Static_assert(HERO_RUNTIME_ABI == 8)` **does not** catch this: a new
header against a stale `runtime.o` links at exit 0 and reads `cmp` from four bytes
past the end of a 40-byte object. Adding a *function* is self-guarding (undefined
symbol at link); adding a *struct field* is not, and the header credits the wrong
mechanism.

The engineer read the tree instead. `types/ops.rs:54` restricts `<` to `int` and
`f64`, and `emit/ops.rs:191` already carries the comment *"§4.14 gives a record no
ordering, so the checker rejected it already"* — so S-desc generates, per reachable
type, **an order the surface cannot spell, cannot test, and cannot observe except
through one built-in's output**. §4.12 answers it in the language's own words: *"If
an operation on `T` is needed, pass it as a parameter."*

And the historian supplied the precedent both were reaching for. Go's `cmp.Ordered`
admits integers, floats and `string` and nothing else; Swift's SE-0266 synthesises
`Comparable` **for enums only, opt-in, structs excluded** — the convener's framing
said otherwise and was wrong; Python 3 *removed* the total cross-type order Python 2
had ("objects of different types except numbers are ordered by their type names"),
which is the exact shape of S-desc and was withdrawn as a mistake. The closing
argument is its own: a structural order over `{K: V}` would be **insertion-order
dependent**, which is why `HashMap` has no `Ord` in Rust while `BTreeMap` has one —
and spec line 58 says a map's insertion order does not affect `==`.

## Resolution — provisional, author ratification pending

### R1 · `sort` emits for `[int]`, `[f64]` and `[str]`. Nothing else, and **no spec token**

S-desc is vetoed. But the narrowing does **not** land in the checker either, and
that is the warden's finding: `{Point: int}` compiles and runs today (verified —
exit 0, one key), so `keys(m)` can be `[Point]`, and spec **line 71** teaches
`for k in sort(keys(m))` as *the* idiom. A checker rule rejecting `sort([Point])`
would make the spec's own sentence a compile error — the `has` failure mode, four
days after paying for it.

So the refusal is the **gate's** (`unsupported[builtin]`, exit 1, "no change to this
file will fix this"), which is the honest state: undecided, not forbidden. The
element-type question goes to **the M6 closure-list audit** with the `sort_by`
finding attached — two seats independently found that the bootstrap's four sorts are
`sort_by_key` over `span.start` (`types/mod.rs:197`, `resolve/mod.rs:195`,
`ir/mod.rs:258`, `emit/gate.rs:108`), which **neither** branch of the proposal
serves. Spec: **+0**.

### R2 · `[f64]` sorts by IEEE 754 totalOrder, not by `<`

Two seats arrived here separately. The engineer: *"NaN is nowhere"* — `grep` finds
it in neither design.md nor the spec, and a `<`-based sort with a NaN yields an
unspecified permutation. The historian: Rust gives `f64` `PartialOrd` and not `Ord`
precisely for this, `total_cmp` (stable 1.62.0) implements totalOrder, and **Go had
to fix it** — `x/exp/slices` documented that Sort *"may fail to sort correctly"*
with NaNs before `slices.Sort` defined NaN-first.

NaN is reachable in Heroes today: the spec's "division by zero aborts" is
implemented for integers only, so `0.0 / 0.0` is a NaN. totalOrder costs ~10 lines,
is deterministic (which the M8c byte-for-byte fixpoint requires), and agrees with
`<` everywhere the spec is observable — so it buys **no spec sentence**.

### R3 · `slice` on `[T]` aborts out of range, identically to `str`

Unanimous. Precedent is Go (*"if the indices are out of range at run time, a
run-time panic occurs"*), Rust's indexing form, and Zig. Python clamps — and the
historian searched CVEs, the CPython tracker and advisories and found **no
documented account of a bug class caused by it**. That is recorded as a finding: the
abort is justified from §1.1, not from a body count.

Closes `tests/golden/unsupported/fixedbugs-array-slice-reached-clang.hero`.

### R4 · The UTF-8 error moves from `chars` to the slice that splits a character

The ergonomist's finding stands and is the strongest single result in this panel:
under the silent spec, `for c in slice("café", from: 0, to: 4).chars()` prints four
lines, the fourth garbage, and the model believes it printed `é`. **No error, no
abort, exit 0.**

Its *remedy* is overruled by three seats, and the mandate decides it:
**design.md:809 — "Slicing that lands mid-sequence is an error."** The convener
verified the defect: `slice("caffè", from: 0, to: 5)` exits **0** today and prints a
corrupt byte. So the error belongs where design.md put it, at the operation that
manufactures the broken string, one line earlier and pointing at the cause.

`chars` therefore stays **total**: a byte that is not part of a well-formed sequence
becomes a one-byte `str`, so **`join(chars(s), "") == s` for every `s`** — an
unconditional law, which is what makes it assertable over `heroes mutate`'s corpus
(CLAUDE.md §9). The ffi-pragmatist's evidence is why this matters beyond elegance:
its §4.19 ladder-step-3 binding read a **LATIN-1 `0xEF` out of a real SQLite `text`
column** (SQLite does not validate UTF-8), so at M7 an aborting `chars` kills a
legitimate binding. Unicode's own conformance clause C10 permits either signalling
or U+FFFD and strongly discourages only *silence*; Heroes signals — at the slice.

### R5 · The spec takes **W7, +11 measured**, and nothing else

```
-`V?` with code `missing_key`. An out-of-bounds array index
-aborts; integer overflow aborts; division by zero aborts.
+`V?` with code `missing_key`. An out-of-bounds index or slice
+aborts, and so does a slice that splits a character; integer overflow aborts; division by zero aborts.
```

It replaces a sentence rather than adding one, which is why it is +11 for two
behaviours: it also documents the `s[9]` abort, which the spec has never stated.

Panel 025 R3's three teeth, applied by their author: the split-character half
**cites a mandate** (design.md:809) and **decides nothing**; the out-of-range half
decides, and carries full burden under panel 012 branch 2 — the pre-registered
prediction below. The third tooth is discharged by goldens on **all five aborts**:
`str` index, array index, `str` slice, array slice, split character. Panel 025 R2's
release condition — *"it lands when `slice` on `[T]` compiles and its abort is
verified on both"* — is met **in the commit that lands the behaviour**, not before.

### R6 · `sort` is a hand-written stable merge sort. Never `qsort`

Measured, and it is a fixpoint argument rather than a taste one. Darwin's `qsort` is
**not stable** — the ffi-pragmatist's probe returned `0/0 0/3 0/6 0/9 1/10 1/1 1/7
1/4` — so two correct hosts would emit two different byte streams and the M8c
`diff B.c C.c` would never close. That is panel 006's reason for the map's fixed
seed, one data structure over. Independently: C11 has no `qsort_r`, the glibc and
Darwin spellings differ in argument order, and passing an `int64_t`-returning
comparator through a cast is C11 6.3.2.3p8 undefined behaviour that
`-fsanitize=function` does not catch.

### R7 · `to_int`'s range check, tested rather than reasoned

```c
if (!(v >= -0x1p63 && v < 0x1p63)) hero_panic("to_int of an f64 outside the range of int");
return (int64_t)v;
```

Negated so NaN falls out for free; hex floats because they are exact; half-open on
the right because `2^63 − 1` is not representable as a `double`. **Two checks a
reviewer would sign off are wrong**: `v <= (double)INT64_MAX` accepts `2^63` because
that cast rounds *up*, and `v > -9223372036854775809.0` rejects `INT64_MIN`. Of 13
probe values, 8 are UB under a raw cast and 0 under this one — and on arm64 the raw
cast does not trap, it *saturates*: `inf` becomes `INT64_MAX` and `NaN` becomes `0`,
silently.

## What a veto would have compelled

Had S-desc landed: `HERO_RUNTIME_ABI` 7 → 8, seven descriptors amended, ~+125 lines
in `perfn.rs` (589 → ~715, **2.4× CLAUDE.md §11's ceiling**), five golden
`.expected` files carrying the ABI number, a recursive containment predicate to
reject maps — and a `SEGV` with no type name and no source line the first time a
generated descriptor omitted one initialiser, which is the failure class panel 022
legislated against.

## Predictions to score

| # | judge | prediction | checkable |
|---|---|---|---|
| 1 | compiler-engineer | under S-scalar: `HERO_RUNTIME_ABI` still **7**, `perfn.rs` still **589**, `descriptors.rs` still **106**, `EMITTED_BUILTINS` **11** entries, `gate.rs` **≤ 295**, `runtime.c` **≤ 986**, **≤ 6** non-test files touched | M6 close |
| 2 | compiler-engineer | at M8 fixpoint, the number of `sort(...)` sites in the self-hosted compiler relying on a *structural* order is **0** — all are sort-by-key or scalar. If ≥ 1, the veto was wrong | M8c |
| 3 | ffi-pragmatist | the §4.19 ladder-3 SQLite binding reaches all six built-ins with **zero shims and zero `HeroDesc` fields added**. Falsifier: any M7 ladder binding needing a C shim *because of* these six | M7 |
| 4 | ffi-pragmatist | if S-desc ever lands, the first generated descriptor missing a `cmp` initialiser is `SEGV (<unknown module>)`, not a diagnostic — already run | on any attempt |
| 5 | llm-ergonomist | the `chars`-on-a-byte-slice shape is silently wrong in **≥30%** of samples under the silent spec and **0%** once it aborts | next harness run |
| 6 | llm-ergonomist | `sort` called on a non-scalar element type: **≥25%** under the silent spec, **≤5%** once stated | next harness run |
| 7 | llm-ergonomist | **counter-prediction against its own approval**: string-encoded sort keys (`to_str(n) + …` fed to `sort`) stay **≤10%**. Above that, the restriction bought a compile error and sold a silent one | next harness run |
| 8 | spec-warden | any `sort` element-type sentence that lands now is **amended or deleted by M8**, because four `sort_by_key` sites need a comparator the closure list has no form for | M8 |
| 9 | spec-warden | spec **≤ 2350 measured** at M6 close | M6 close |
| 10 | historian | if `sort` admits `f64` through `<` rather than a totalOrder, a NaN produces a not-sorted output with no error — as `x/exp/slices` documented before Go fixed it | this step |

Scored here: panel 026 #6 (count < 2450 through 2026-09-30) **alive** — 2345 leaves
105. Panel 025 #6 second clause **holds** (the last amendment was −13, a removal).
Panel 025 #7 (three amendments > +75 with zero removals) **does not fire**: +43,
−13, +11. Panel 026 R8's rule holds for the **fifth** recorded time — the slice
sentence priced at +9 on base 2304 re-measures **+10** on base 2334 for identical
text.

## Watch list

- **`runtime/runtime.c` is 866 lines**, the largest CLAUDE.md §11 breach in the tree
  at 2.9×, and this step adds ~120 more. The split is queued as its own commit
  inside M6, before anything else lands in that file.
- **A dead emitter arm goes live.** `emit/ops.rs:169-172` already emits
  `hero_str_cmp(l, r) < 0` for `<` on `str`, and `types/ops.rs:54` rejects it — so
  that code is unreachable today. `sort([str])` gives `hero_str_cmp` its first live
  call site while `a < b` on the same two strings stays a compile error. The
  engineer costed removing the asymmetry at **one token** in `types/ops.rs`; it is a
  §4.14 change and belongs to the audit, not to this step.
- **`measure/` vendors two tokenizer tables**; panel 011 counted three, and o200k was
  the *highest* on spec v0 (2050 against cl100k's 2048). The binding maximum may
  understate by ~0.1%. Immaterial today, recorded so nobody finds it at 4000.
- **design.md contradicts itself on `sort`'s tier**: §1.0:139 puts it in the built-in
  half of the closure list, design.md:1382 lists it among the functions generics turn
  into library code. That contradiction *is* the element-type question, and the audit
  inherits it.
- **`join`'s arity**: design.md:1318 names `join([str]) -> str`; the tree implements
  `join(parts, separator)`. The tree is ahead of the document; the audit records
  which is canonical.
- **Two gaps the ergonomist hit in both variants, both silent, neither addressed**:
  `len` on a `str` counts bytes or characters (unstated), and `range(a, b)` includes
  `b` or not (unstated). It ranked the first fourth of seven overall.
- **The delta gate and the mortgage ledger** (panels 024, 026) are still unbuilt.
  W7 at +11 passes the gate the warden proposed; E (+27) and F (+39) would not have.

## DESIGN-LOG

Appended 2026-08-11 — see the lines citing panel 027.

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
