# Panel 023 — a type with no size

**Convened** 2026-08-10, before M5c step 1 (the descriptor pass).
**Trigger** CLAUDE.md §4 — a diagnostic *class*.
**Status** provisional — author ratification pending.

Heroes has no `Box`, no `ref`, no pointer type. Records are C structs by value and
variants are tagged unions by value, so a recursive type is finite only when its
recursion passes through something that holds its contents elsewhere. Nothing in
the compiler checks that today, and nothing had to: `emit/gate.rs:155` refuses
records and variants entirely. M5c removes that row, and on the day it does,
`record Node { child: Node }` stops being accepted and starts being **exit 2, an
internal error, with a path to generated C** — this compiler telling the author
that the compiler is broken about a program the author got wrong.

Measured before the proposal was written: `heroes check` exits **0** on all four
of the shapes below.

```
record Node                record A            record R            record F
    child: Node                b: B                x: R?               f: (function(F) -> int)
                           record B
                               a: A
```

## The proposal, verbatim

> A new error class in the type checker, code `no_size`, reported for any record or
> variant whose field graph has a cycle that does not pass through `[T]` or
> `{K: V}`. Reported once per cycle, at the field that closes it, with the cycle
> spelled out in the message and a `guess` Fix suggesting `[T]`.
>
> Plus two lines in the spec's Types section, measured at **+40** (2225 → 2265):
>
> ```
> - A type contains itself only through `[T]`: `variant Expr` with a case field
>   `children: [Expr]`. A record or variant holding itself directly has no size.
> ```

Both halves of that proposal are wrong, in opposite directions, and the panel
measured both.

## The verdict table

| judge | verdict | its own finding | delta / cost | condition |
|---|---|---|---|---|
| compiler-engineer | accept-with-condition | design.md §3.1:461 **already** orders this relation computed; `record R { x: R? }` is infinite and the proposal never names it | ~150 non-test + 100 test; new `types/sized.rs` | **one walk, one home**; rows listed, never a catch-all |
| llm-ergonomist | approve | 3 of 3 "the spec does not say"; would have called `child: Node` legal; doubted `[Node]` is legal **at all** (~75%) | +40 worth it, but the wording is broken | "directly" re-licenses mutual recursion, which spec line 38 invites |
| spec-warden | **object** | the proposal contradicts itself on `{K: V}`; the same rule costs **+8** in the `[T]` row | +40 verified exactly; recommends +8 | veto if the sentence lands still contradicting the diagnostic |
| ffi-pragmatist | **object** | compiled it: `{str: Scope}` is **legal** (8 bytes), `next: R?` is **illegal** — the rule is wrong in both directions | ABI: **nothing**, and it cannot be otherwise | veto if any cycle is rescued by changing the representation |
| historian | approve (advisory) | the rule is **Go's**, verified in Go's own test data; the diagnostic shape is **Swift's**; the fix is a `guess`, because rustc's is | — | revise if Hylo documents silent boxing |

## Where the judges converged, and on what

**Two judges found the same missed row from opposite ends.** The ergonomist,
reading only the spec, named `next: Node?` "the single most attractive wrong
answer" for an optional child — spec line 53 says *"Absence is a different type
(`T?`)"* while line 50 says a `T?` is *"a `T`, or an error"*, so the spec itself
licenses `T?` as an option type. The compiler-engineer, reading only the tree,
found the same shape as a representation defect: a `T?` is by value, so a cycle
through one is infinite. Then the ffi-pragmatist compiled it: `field has
incomplete type 'h_m_R'`, exit 1. **The proposal's rule would have permitted it**,
and the resulting error would have carried no line pointing at user source.

**Nobody defended the token count.** The warden priced the identical rule at +8 in
the existing `[T]` table row and observed that 32 of the 40 tokens buy prose. The
ergonomist, asked to vote on +40, voted for it — but its stated reason was
*licensing the legal form*, which the +8 row does equally.

**The historian settled why this error must exist at all.** OCaml and Haskell do
not have it, and the reason is not that they are cleverer: the OCaml manual states
that the default representation is boxed, so every field is already pointer-sized
and the question never arises. The proof that this is the whole explanation is
that the question **reappears in OCaml exactly where unboxing is requested** —
`type t = A of t [@@unboxed]` is accepted today and, in the maintainers' words,
*"this type is inhabited by no OCaml runtime value"*; ocaml/ocaml#10485 proposes
rejecting it. So: **the error is the price of unboxed value aggregates, not a
property of recursive types**, and Heroes has already bought unboxed aggregates
(§4.10). Every unboxed-value language in the historian's table has this error.
The two that don't are boxed-uniform, which Heroes is not.

## Resolution — provisional, author ratification pending

### R1 · The class lands, in the checker

Code `no_size`, `Kind::Error`, raised in `crates/heroes/src/types/`. Not in the IR
and not in the emitter: the precedent is `missing_return`'s own commit body —
*"written first in the IR … then moved to the checker: a diagnostic `heroes check`
cannot see is a diagnostic in the wrong pass."*

Principle 0 is satisfied by its second clause, and both judges who checked agree
the first clause fails: the self-hosted compiler does **not** need a
self-containing type. `syntax/ast/mod.rs:14-22` says so in the author's own words
— *"Types are recursive … so they all live in one arena, `Ast::types`, and point
at each other with `TypeId` — an index, never a pointer. The port reads `types:
[TypeNode]` unchanged."* The rule earns its place by converting an exit 2 that
blames the compiler into an exit 1 that names the author's field.

### R2 · The edge set is a **property**, not a list — the ffi-pragmatist's objection

> **An edge is a field C lays out by value.**

Stated as a list of permitted indirections, the rule is wrong twice, and both
errors were compiled rather than argued:

| shape | proposal's rule | clang, §7 flag set | correct rule |
|---|---|---|---|
| `record Scope { vars: {str: Scope} }` | legal | **exit 0**, 8 bytes | legal |
| `record R { next: R? }` | legal | **exit 1**, `field has incomplete type` | **illegal** |
| `record F { f: (function(F) -> int) }` | illegal | **exit 0, and it runs** | legal |
| `record E {}` | — | already `error[empty_record]`, exit 1 | no work |

A `T?` is **transparent**: it propagates the edge into `T` rather than breaking
it. Measured, all three: `record P { kids: [P?] }` is 8 bytes, `record Q { kids:
[Q]? }` is 40, `record S { m: {int: S?} }` is 8 — every one compiles. The property
gets all four rows right without naming any of them, and it cannot drift when M6
adds function values.

The ffi-pragmatist's framing is worth keeping verbatim, because it says where the
rule comes from: C11 6.7.2.1 forbids a member of incomplete type, so a by-value
self-referential C struct **cannot exist in any header** — measured on the real
ones, `sqlite3.h` has 23 self-referential fields and **0 by value**, and
`sqlite3_vfs` alone has 16 of them as function pointers. **This rule is C's own
rule, moved earlier and said in Heroes' words.**

### R3 · One walk, one home — the compiler-engineer's condition

The cycle check and the emitter's `typedef` ordering are the same relation, and
design.md §3.1:461 has ordered it computed since before the backend existed:
*"Emit prototypes for all functions and `typedef struct`s topologically sorted by
by-value containment … in deterministic order."* So the walk lives in
`crates/heroes/src/types/sized.rs`, publishes its order in `Checked`, and the
emitter **filters** it to the descriptor worklist's reachable subset — a total
order restricted to a subset is still a total order, which is one `if` in a loop.
`emit/` contains no graph code; `grep -E 'topolog|scc|postorder|visited'` over
`emit/*.rs` must return nothing.

This is what forced R2. Two edge sets cannot be one walk: a map imposes **no**
ordering constraint on C — measured, three records mutually recursive through
arrays compile in the worst order with **zero forward declarations**, because
`HeroArrayHeader *` is type-erased — so the sort must not carry a map edge, while
a narrow rule would need one to reject the cycle. The warden's and the engineer's
veto triggers are satisfied by the same choice, and only by it.

Also measured, and the reason the sort is mandatory rather than nice: a forward
`typedef struct T T;` does **not** rescue a by-value field. Same two errors with
and without it.

### R4 · The walk is **iterative**

Not a style preference — a verified failure mode of this exact check in four
compilers. rustc #84611 is a stack overflow *inside* the recursion check; #74201
"builds forever"; #81867 is an infinite-size type that escapes to MIR; Nim #13715
is a SIGSEGV; Zig #21436 is an undetected cycle plus a crash. The check that
prevents a hang is the one that historically hangs.

### R5 · The diagnostic shape is **Swift's**

Swift is the only precedent in Heroes' exact position — a value type with no
escape hatch — and its shape, from its own Sema tests: the error is on the
**field**, not the type header, and the note carries the cycle as a **named path**
(`IndirectlyRecursiveStruct2 -> (a: IndirectlyRecursiveStruct1)`).

Heroes takes: one diagnostic per cycle, primary span on the field that closes it,
one note per edge naming `Type.field` and its line — because in `A → B → A` both
edges are legal repair sites, and a message that names one silently is arbitrary
advice dressed as help. For annotation stability under CLAUDE.md §9: DFS roots in
`ast.decls` order, and the printed cycle normalised to start at the lowest decl
index.

Rejected: one diagnostic per *type* (breaks "one mistake, one message",
`types/lower.rs:8-11`) and one per program (untestable — there is no line to hang
`#~ no_size` on). Go's own history is the argument for the field span: the old
`cmd/compile` printed only `invalid recursive type T2` for a mutual pair and that
was **filed as a bug** (#41575), then unified to the path form.

### R6 · The fix is `guess`, never `certain`

CLAUDE.md §8 machine-applies only `certain`, and this fix must never be applied
without a human. rustc's `Box` suggestion for E0072 is emitted with
`Applicability::HasPlaceholders` — rustc's own definition: *"cannot be applied
automatically because it will not result in valid Rust code"* — so `cargo fix`
leaves it alone. Swift, with no struct escape hatch, emits **no fix-it at all**
for structs and reserves one for enums where a keyword suffices. And the local
reason is decisive: `child: Node` → `children: [Node]` changes the field's
cardinality *and* its name. Every use site changes with it.

### R7 · The spec pays for the **licence**, not the prohibition — +6 measured

There is an asymmetry nobody had named, and it decides the token count:

> A diagnostic can only teach you at the moment you are wrong. It can never tell
> you that something is **allowed**, because a legal program produces no
> diagnostic.

So the spec buys only the half the diagnostic cannot reach. What lands, in the
existing `[T]` row:

```
| `[T]` | dynamic array, indices from 0; how a type contains itself |
```

**2225 → 2231, +6 measured, headroom 769.** Every candidate was measured jointly
against the real baseline, never by adding separate deltas:

| candidate | binding max | delta | why not |
|---|---|---|---|
| nothing | 2225 | +0 | leaves the ergonomist's worst finding standing (below) |
| the `[T]` row, "how a type contains itself" | **2231** | **+6** | **adopted** |
| the row, "the only way a type contains itself" | 2233 | +8 | "only" is **false** — `{str: Scope}` compiles |
| the proposed bullet | 2265 | +40 | false about `{K: V}`, and re-licenses mutual recursion |
| the bullet, corrected for mutual recursion | 2274 | +49 | same falsehood, 43 tokens more |
| the property stated in full | 2262 | +37 | correct, and 31 tokens for a case a reader rarely writes |

The ergonomist's worst finding is the one this buys off, and it is not the one the
proposal was about. Reading only the spec, it put ~75% confidence on
`children: [Node]` being legal **at all** — because line 60 says *"No aliasing
exists anywhere"*, which reads as *no indirection anywhere*, and the only in-spec
support for the array form is line 97's `s.children`, an example **whose types are
never written down**. Its prediction: 10–20% of current-spec samples hand-roll an
index arena to dodge a restriction that does not exist. That is a **silent**
divergence — no error, a program four times the size — and it is the only failure
mode in this panel that no diagnostic can ever catch.

### R8 · design.md §3.1:461 is amended, and it is the only ratified text that was wrong

*"(cycles are legal only through `[T]`)"* → the by-value property. This is the one
place where the ratified design was narrower than the backend it describes, and
`{str: Scope}` compiling at 8 bytes is the measurement that says so.

### R9 · The accepted case goes in **before** the check is written

The historian's first prediction, which is a gift rather than a forecast: **the
first defect will be a cut-edge bug, not a cycle-detection bug.** A checker that
marks types "visited" instead of treating `[T]` as an edge it does not traverse
will wrongly reject

```
record A            record B
    xs: [B]             a: A
```

— a cycle that passes through `[T]` exactly once, and legal. Go's `A5 [10]A6 /
A6 *A5` is legal in `cycles0.go` for the same structural reason, and every
over-eagerness bug the historian found in rustc (#68748, #144617, #31299) is a
false positive, never a false negative. So this program enters the corpus as an
**accepted** case in the same commit as the check.

### R10 · Three corrections found while measuring something else

1. **§4.20's "`T?` is 40 bytes for every `T`" is false.** `sizeof(Big?) == 48`
   for a 40-byte record: it is 40 while `T` ≤ 32 bytes. Measured by the
   ffi-pragmatist while compiling something else.
2. **DESIGN-LOG line 107 records panel 022 at "+24 measured (2196 → 2220)"; the
   file measures 2225.** Commit `280555c`'s own body agrees with the warden:
   *"+29, where the three deltas measured separately sum to +24."* Separately
   measured deltas understated by 21%. This is the **fourth** recorded instance in
   this project of measured deltas not composing, and the correction is appended
   rather than edited, because the log is append-only.
3. **A payload-free variant emits an empty union.** `variant Color { red | green
   | blue }` produces `union { } as;`, which compiles at `-Wall` and is
   `error: empty union is a GNU extension` under `-pedantic-errors`, which §4.19
   schedules. Omitting the union entirely compiles clean under both, size 4 either
   way. M5c work; queued.

### What a veto would have compelled

The warden's, had the +40 landed with the `{K: V}` contradiction intact: a spec
sentence contradicting its own diagnostic on day one, which CLAUDE.md §12 resolves
against the compiler — so the compiler would have been wrong by construction.

The ffi-pragmatist's, had any cycle been rescued by changing the representation:
auto-boxing a recursive field is `Box` in disguise. It puts an indirection inside
a record §4.10 says has none, changes layout, and re-opens the cycle-collector
question §4.10 gift 2 closed. The compiler-engineer's veto (b) is the same veto
from the other side, and it is the one point on which all three vetoing judges
agree without having been asked.

The compiler-engineer's, had the order been computed twice: two answers to one
question, arriving in two passes, with the double-emit determinism test green
either way — panel 022's failure shape exactly.

## Predictions to score

| # | judge | prediction | checkable |
|---|---|---|---|
| 1 | compiler-engineer | `types/sized.rs` ≤ 150 lines; the class's non-test diff under `types/` ≤ 180 | M5c close |
| 2 | compiler-engineer | `grep -E 'topolog\|scc\|postorder\|visited' emit/*.rs` returns 0 | M5c close |
| 3 | compiler-engineer | no program in `tests/golden/` exits 2 | M5c close |
| 4 | compiler-engineer | `no_size` contributes **zero** to Metric 3's differential — it kills in both arms, so it must not join `is_thesis_rule`'s 13 codes | next harness run |
| 5 | llm-ergonomist | arity-unstated tasks: first-try declaration rate ≤40% without the spec line, ≥85% with it | next harness run |
| 6 | llm-ergonomist | **arity-stated tasks measure ≈0 delta** — a harness that only asks for "any number of children" will wrongly conclude the line is free to drop | next harness run |
| 7 | llm-ergonomist | ≥25% of samples reach for `next: T?` under **both** specs; spec line 53 is the cause, not the new line | next harness run |
| 8 | ffi-pragmatist | no `extern` in §4.19's SQLite ladder mentions an aggregate, so `no_size` never runs on an FFI type | M7 |
| 9 | ffi-pragmatist | `variant Color { red \| green \| blue }` passes CI today and fails the day `-pedantic-errors` lands | whenever that lands |
| 10 | historian | the first defect is a **false positive** on `record A { xs: [B] } / record B { a: A }`, not a missed cycle | M5c step 1 |
| 11 | historian | a type-header-only span produces "which field?" as the first follow-up | M5c close |

## Watch list

- **Spec line 53 is a defect adjacent to this panel.** *"Absence is a different
  type (`T?`)"* invites `T?` as an option type, while line 50 defines it as *"a
  `T`, or an error"*. The ergonomist predicts ≥25% of tree programs reach for
  `next: T?` under **either** spec. Panel 023 does not fix it and must not be
  credited with it.
- **Two files are over §11's ceiling before M5c adds a line**: `emit/inst.rs` at
  394 and `types/tests/data.rs` at 316. The descriptor pass needs a new
  `emit/types.rs` regardless.
- **A new mutation operator is available**: `unbox-field`, rewriting
  `children: [Expr]` → `children: Expr`. It kills in both arms, so it is data
  about the checker rather than about the thesis.
- **Unverified, and inadmissible until sourced** (the historian's own list):
  rustc's applicability for the `Box` suggestion in current master (verified only
  at PR #91416, 2021); the existence of rustc's plural `recursive types A and B`
  form; the C11 paragraph number (6.7.2.1 confirmed, the paragraph not); **Hylo's
  `indirect` semantics** — the keyword is in the specification's identifier list
  and its meaning is absent, and given §4.10's Hylo lineage this is the gap worth
  chasing.

## DESIGN-LOG

Appended 2026-08-10 — see the three lines citing panel 023.
