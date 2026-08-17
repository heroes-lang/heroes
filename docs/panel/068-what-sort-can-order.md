# Panel 068 — what `sort` can order, and which artifact had the bug

**Lane: full** (compiler-engineer, llm-ergonomist, spec-warden, ffi-pragmatist,
historian). Convened and synthesised 2026-08-16.
**Provisional — author ratification pending.**

**Verdict: the spec has the bug, not the compiler.** Five seats, **five vetoes
or rejections of A**, five approvals of B. `sort` keeps its scalar-and-`str`
domain; `spec:85`'s promise is repaired; `sort_by` is priced, refused today,
and left available with a named return condition.

## What was asked

`spec/heroes-spec.md:85` says: *"`keys(m) -> [K]` gives the keys, so `for k in
sort(keys(m))` walks in order."* `sort` is listed at `spec:168` with no element
restriction. But `sort` on `[Record]` type-checks and dies in the emitter, and a
compound map key **must** be a record because Heroes has no tuples — so the
spec's own map-walking idiom is unavailable for exactly the maps that need it.
CLAUDE.md §12: spec beats compiler, and one of them has the bug. **Which?**

Candidates: **A** `sort` orders records structurally · **B** the checker refuses
with a reason and the spec is repaired · **C** `sort_by(xs, less)` · **D** any.

## The convener briefed a false premise, for the third sitting running

The brief told the FFI seat *"sort.c already solved NaN ordering once
(totalOrder)"*. **It did not.** `runtime/parts/sort.c:37-46` rejects totalOrder
in writing and **aborts** instead — measured, `panic: sort of an f64 array
containing nan`, exit 134. `DESIGN-LOG:148` recorded totalOrder as panel 027 R2
and **`DESIGN-LOG:205` superseded it four days later**. A judgement built on the
brief would have rested on a document that had already stopped being true.
Panels 066, 067 and 068: three briefs, three wrong premises, three catches by
the seats.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **ffi-pragmatist** | **veto A** · approve **C** conditioned · B as C's interim half | **It ran the A experiment and its own panel-027 prediction #4 fired.** Adding a sixth `HeroDesc` field and compiling the *unchanged* runtime under this project's `FLAGS`: **exit 0, no diagnostic, 16 descriptors silently NULL** (027 measured 7 — the blast radius more than doubled). Then the worse half: under version skew the stale slot reads **`0x4`, not NULL**, so the runtime's own `if (cmp == NULL) hero_panic(…)` guard **does not fire** → **SIGSEGV, exit 139**. *"A `cmp`-in-descriptor design cannot be made safe by checking the slot, because the failure mode that matters produces a plausible non-null garbage pointer."* And **design.md §4.20 already rules this**: *"the descriptor has five members and gains no sixth… adding a function to this ABI is self-guarding — an undefined symbol at link — while adding a struct field is not."* Option C measured on the right side of that asymmetry: `sizeof(HeroDesc)` **40 unchanged**, ABI **14 unchanged**, +30 runtime lines, and skew fails as `Undefined symbols: _hero_array_sort_by` |
| **compiler-engineer** | approve **B** · **veto A** · C admissible, not this sitting's | **`==` is total over Heroes types; `<` cannot be.** A record may hold a map and a function value — **both compiled**. A map has no canonical order without sorting its own keys (allocation inside a comparator); a function value is a link-time address, and C11 6.5.8p5 leaves `<` on pointers into distinct objects undefined, so the answer **moves with the linker** — the nondeterminism the fixpoint exists to exclude. Therefore *"A must ship a refusal rule for records that transitively reach a map, function, `ptr` or `cstr` — **and that refusal rule is B**. A is B plus ~250 emitter lines plus several invented orders. It is strictly dominated."* Priced even in its cheapest ABI-free form (comparator as a parameter) and still vetoed, on semantics |
| **spec-warden** | approve **B** · **veto A** · **refuse C**, priced | **The decision was already taken and ratified; the spec is what lagged.** Panel 027 R1 put `sort` at the scalars at **+0 spec tokens**, and design.md:2232 carries it normatively on compiled evidence. *"The compiler is not lagging a decision; it **is** the decision."* Measured the repair and wrote it: **+22 net** (+34 gross against a **−12 named removal** — `spec:85`'s false derivation), 3377 → **3399**. A costs **+72 to +108**. And **a second false sentence one row away**: the operator table lists `< <= > >=` with no restriction while `"a" < "b"` is `bad_operand` and `sort(["b","a"])` runs — so the row is repaired too |
| **llm-ergonomist** (spec-only, blind) | approve **C** · **veto A** | **Wrote the failing program without hesitating, and got 18 lines in before the first build.** *"The spec did not merely fail to warn me — it gave me positive evidence"*: `spec:216` says a `partial` record cannot be a map key, and naming the one record kind that cannot implies the others can. **Against A**: today field order means nothing — construction is named and `Point(3, 4)` **does not exist** — so A makes a cosmetic reorder change the output of every program that sorts that record, in files that never mention it. And a record holding a `ptr` would sort **differently on every run**. **Against C, honestly**: `a.x < b.x || a.y < b.y` builds, type-checks and mis-sorts — C moves the failure from *will not build* to *builds and is wrong* |
| **historian** (advisory) | approve **B + C** as a pair | **A in its stated form has no precedent.** Haskell, Rust and Swift all have declaration-order lexicographic ordering and **all three make it opt-in per type**. *"The panel's option A is not 'the Haskell/Rust choice'; it is the OCaml/Python-2 choice wearing Haskell's clothes."* The languages that shipped an implicit universal structural order are the three known failures: OCaml's polymorphic compare (shunned by its own community's stdlib), Python 2's arbitrary order (**removed** in Python 3), JavaScript's stringify default. **Two institutional judgements on A's hazard**: Rust classes reordering fields under derived `Ord` as **semver-incompatible** (#109946), and Swift declined `Comparable` synthesis for structs on the *"all fields are reorderable"* principle (SE-0266). **But B alone has a documented cost**: Elm has Heroes' exact restriction and produced elm/compiler#774, a decade-long thread, because programmers substitute `String` keys and lose the type checker |

## Where the seats disagree

**Only on C's timing, and the split is 3–2.** The ergonomist, historian and
ffi-pragmatist want `sort_by` now; the compiler-engineer calls it admissible but
not this sitting's, and the spec-warden refuses it today on Principle 0 —
*"nothing since has needed it"* (measurement 003:126, still true: the six
`sort_by_key` sites in the bootstrap all key on an `i64` span). **The
conservative resolution adopts B alone and leaves C priced with a return
condition**, which is the panel rule's own default and matches the two seats
that measured the compiler rather than the surface.

## What the seats measured that nobody asked for

- **`sort` accepts 11 element types, not 3** (8 integer widths + 2 floats +
  `str`) — the diagnostic's *"other than `i64`, `f64` or `str`"* has been stale
  since panel 042. **`<` accepts 10**, and its message says *"`i64` or `f64`"*.
  A test asserts the wrong wording verbatim.
- **The refusal's diagnostic contradicts itself**: it lists `sort` among what
  the backend *does* emit, in the same message that refuses `sort`. And *"not
  emitted **yet**"* becomes a permanent lie under this verdict.
- **`bool` is the one scalar the spec implies and the compiler refuses** — and
  sortability is already **not** `<` in this language, since `sort([str])` runs
  while `"a" < "b"` does not.
- **All 7 pair-keyed `// ORDER:` marks nest** — every one is *group by the
  first component, then order by the second*, which `{str: {str: V}}` already
  solves. 2 of 7 ported, both hold. **But A would cover only 3 of the
  bootstrap's 6 compound keys**: an array key and a variant key get nothing
  from it, while `sort_by` covers all six.
- **The FFI gets zero from A.** The canonical binding-side record-keyed map is
  the glyph atlas, and `{GlyphKey: ptr}` is refused **for its value** by panel
  067's own fix. A binding never contains a map at all.
- **An expired premise cited in five places** (`emit/builtins.rs`,
  `emit/tests/gate.rs`, a golden's header, `DESIGN-LOG:147`,
  `selfhost/resolved.hero`), two of which cite *"line 71"* when it is line 85.
- **The ledger has an unrecorded row, one commit old**: panel 066 moved
  `SPEC_TOKENS` 3374 → 3377 and added no ledger row. That is the convener's.

## Two live defects, both found while answering something else

**1. The seventh shape of panel 067's F7.** `ptr`/`cstr` reached **through a
record field**, where the record is a map key: `heroes check` exit 0, then
`panic: entered unreachable code` at exit 134. The emitted C says it plainly —
`h = (h ^ (hero_unreachable(), UINT64_C(0))) * …` inside the generated `_hash`.
Panel 067's fix covered six shapes with `ptr`/`cstr` as the **direct** element;
`emit/gate_types.rs::check_element` does not descend into a record's fields.
Only the **key** position fires, because only there is `hash` called. **The
project already owns the correct transitive walk** — `types/partial.rs` descends
`[T]`, `{K: V}`, `T?`, record fields and variant payloads for exactly this
hazard — and it was not reused.

**2. A function-typed record field emits broken C at exit 2.** `g.f(1)` where
`f` is a record field of function type: `incompatible pointer to integer
conversion`. Cause is `ir/calls.rs:172-176`, typed with the *call's* result
because *"function values land at M-generics-library, which is where this path
is first exercised"* — **a premise that expired when that milestone landed**.
Every function type in `tests/golden/` is a parameter, never a record field.

## The resolution — provisional, author ratification pending

1. **A is vetoed**, and by five seats. It does not return without amending
   design.md §4.20's *"gains no sixth"* by name, in a sitting that re-runs the
   NULL-slot measurement.
2. **B lands**: the refusal moves from the emitter to the **checker**, naming
   the ordered set and why, with the walk **transitive** over record fields and
   variant payloads (reusing `types/partial.rs`'s existing descent). `bool`
   joins the sortable set. The message must not say *"has no `<`"* — false of
   `str`, which sorts — and must be worded so C remains addable.
3. **The spec is repaired at both false sentences**, in the warden's measured
   wording: `spec:85`'s derivation removed, `spec:168`'s built-in entry gains
   its restriction, the operator row gains `(< <= > >=: a number only)`.
   **3377 → 3399, +22 net against a −12 named removal**, which is panel 012
   branch 1 — a payment, not a promise.
4. **C is refused today and left available**, priced at +47…+55. Its return
   condition is the warden's registered prediction below.
5. **The two live defects are fixed** — the seventh shape in this sitting, the
   function-typed field in its own.
6. **Panel 066's missing ledger row is backfilled** in the same commit.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| spec-warden (the refusal's falsifier) | all **7** pair-keyed `ORDER:` marks port to nested maps with a number or `str` at every level, and **zero** need an order on a record — falsified by one whose port record names the nesting as a loss | M-selfhost-port close, again at fixpoint |
| compiler-engineer | **0** sites need a structural record order; **≥5** need a stable sort of an aggregate by one field **with ties** (the `span.start` sorts, where two diagnostics are pushed with the same span) | M-selfhost-fixpoint |
| ffi-pragmatist | if C lands: `sizeof(HeroDesc)` stays 40, ABI stays 14, runtime delta ≤40 lines. On any attempt at A: no diagnostic under `FLAGS`, a non-NULL garbage slot, the NULL guard silent — **already run: exit 0, `0x4`, exit 139** | M-generics-library · any |
| llm-ergonomist | a model writes `sort(keys(m))` first in >9 of 10 trials on a record-keyed map; if C ships with a one-field example, ≥1 of the first three predicates is written as the invalid `a.x < b.x \|\| a.y < b.y` | measurable now · M-module-namespace |
| historian | if B lands without C, the compiler needs ≥1 deterministic compound-key walk before the fixpoint and implements it by string-encoding or a hand-rolled sort — elm/compiler#774's substitution | M-selfhost-fixpoint |

**Scored in this sitting**: the spec-warden's own panel-027 #8 (*"any `sort`
element-type sentence that lands now is amended or deleted by M8"*) is **vacuous
on its antecedent** — R1 spent +0, so no sentence landed — and its reason clause
is **falsified in the direction supporting today's verdict**. Recorded by that
seat rather than quietly dropped: *"I wrote that prediction expecting A to
become necessary, and the port's evidence says it did not."*

## Author's verdict

**Ratified as it stands, 2026-08-16** (author instruction, *"Ratifico il panel
68"*, after the plain-language recap of all five seats). Everything provisional
above becomes final:

- **`sort` keeps its scalar-and-`str` domain.** A is refused by all five seats
  and does not return without amending design.md §4.20's *"gains no sixth"* **by
  name**, in a sitting that re-runs the `0x4` measurement.
- **The spec repair stands as landed** — 3377 → 3399, +34 gross against the
  −12 named removal, both ledger rows written (this sitting's, and panel 066's
  backfill).
- **C is refused today and available**, at +47…+55, with the spec-warden's
  registered prediction as its return condition.
- **B's remaining half is owed**: the refusal moves from the emitter to the
  checker, transitive, admitting `bool`, worded so C stays addable. The
  transitive walk landed with the seventh-shape fix; the *move* has not.

**What was already landed before ratification** — the seventh shape of panel
067's F7 (`ptr`/`cstr` reached through a record field or a variant-case payload,
where the record is a map key: `check` exit 0, then abort at 134), because it is
a §1.12 crash and both the compiler-engineer and the ffi-pragmatist made fixing
it condition 1 regardless of which option won.


## Predictions scored at M-selfhost-port close (2026-08-17)

**The spec-warden's refusal-falsifier holds in its consequence, and its
mechanism was a third answer neither branch named.**

*"Zero need an order on a record"* — **zero**, measured over all 143 modules:
no map in the port is keyed by a record, so the refusal this row guards was
never reached and the falsifier never fired.

*"All 7 pair-keyed `ORDER:` marks port to nested maps"* — they did not, and
nothing was lost. The port encodes the pair into one `i64` (`emit_ctype`'s
`case_key(decl:, at:)`) rather than nesting two maps, so a composite key
becomes a number and `sort(keys(…))` orders it directly. One site, not seven:
the other six marks turned out to be single-keyed once the port reached them.

The prediction asked to be falsified by *"one whose port record names the
nesting as a loss"*. None does, because none nests.
