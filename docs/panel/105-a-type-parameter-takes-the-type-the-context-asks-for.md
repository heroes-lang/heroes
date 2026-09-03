# Panel 105 — A type parameter takes the type the context asks for

Date: 2026-09-03, evening. Full panel, five seats, differentiated by input. The
defect is `docs/defects/006`, found the same day by M-corpus-depth step 5 while
`examples/interpreter/run/value.hero` was being written. Coordinator: the
assistant. The tree every seat measured is a frozen snapshot of `756b3920` plus
one unrelated uncommitted runtime change (the repair of defect 007), copied once
per seat into the scratchpad; the working tree was not touched by any seat.

**Resolution: Option A, flat — provisional, author ratification pending**, with
every condition of every approving seat and both ruling-independent repairs
(§ Resolution). The author's standing instruction for this work, given twice the
same evening and recorded in `DESIGN-LOG.md` under 2026-09-03 (*"le soluzioni
più robuste e complete rispetto a quelle più economiche"*; *"voglio quella più
robusta, non transigo"*), is the criterion the synthesis applied where the seats
disagreed on cost.

## The proposal, verbatim

The spec line today (`spec/heroes-spec.md` § Functions and calls):

> Generics: on functions only, no constraints, always inferred, never written
> at the call site: `function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]`.

Nothing in the document says WHERE inference looks. design.md §4.12 says *"look
at argument types, deduce `T`"*. The program that found the gap:

```
function wanted<A>(what: str) -> A?
    return fail("type", what)

function number_of(v: Value) -> i64?
    return match v
        .number n => ok(n.v)
        .text _ => wanted(what: "a number")
```

`heroes check` exit 0; `heroes build` exit 134, `assert failed: false`; as a
`return`'s value instead of a `match` arm's, exit 2 from the verifier.

**Option A — infer from the type the context asks for, after the arguments.**
Spec +32 tokens, measured 3718 → 3750 (the spec-warden's ladder found +13 for
the mechanism alone and +20 with the error clause; the ergonomist judged this
exact wording and it is kept):

```
- Generics: on functions only, no constraints, always inferred, never written
  at the call site: `function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]`.
  A type parameter takes its type from the arguments, else from the type the
  context asks for; a call that says neither is an error.
```

**Option B — refuse at the declaration.** Spec +17 (floor +3):

```
- Generics: on functions only, no constraints, always inferred from the
  arguments, never written at the call site:
  `function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]`. Every type
  parameter appears in a parameter's type.
```

**Under either option two repairs were owed**: the IR verifier catches the
`match`-arm spelling as it catches the `return` spelling, and the emitter's
`assert failed: false` becomes a message.

## The verdict table

| seat | input | verdict | judged from | cost / delta measured | prediction | conditions |
|---|---|---|---|---|---|---|
| **llm-ergonomist** | the spec alone, the two variants label-stripped, three writing tasks | **A: APPROVE-WITH-CONDITIONS · B: OBJECT** | the two sentences themselves | wrote the program cold: at two accessors inlined `fail`, at three or more reached for the result-only helper — *"there is no non-generic way to factor a failure in this language"*; and `fail`, `nullptr`, `[]`, `{}` are ALREADY context-typed in the spec, so B forbids user functions what the builtins do | A: in a harness task with ≥3 fallible accessors, ≥30% of first tries declare a result-only helper, ≥90% of those compile, the no-context error fires in ≤5%; falsified at ≥20% (the sentence being read HM-style). B: 100% of helper-declaring first tries fail at the declaration, median repair ≥N lines against 1 | (1) the no-context diagnostic names the parameter, says it appears only in the result, offers the same-line annotation as a `guess` fix, and for `print(...)`/`_ = ...` offers bind-first — because `helper<i64>(...)` is the model's next move otherwise; (2) context reaches every `match`/`if` arm in either order; (3) `x = helper(...)` with no annotation is an error even when a later line would fix the type — *"inference is local only"* holds, or the seat vetoes |
| **compiler-engineer** | the proposal + pointers into `selfhost/`, the seed route, the copy rule | **A: APPROVE (conditions) · B: OBJECT** (no veto: under the ceiling, no core construct) | §1.7, Part 5 (`design.md:2336`), §1.3/§4.5 (locality; the brief's "§1.5" was the wrong section) | A ≈ 75–90 non-test lines, ALL in the checker (`walk.hero` ≈ +35, `generics.hero` ≈ +30, `contextless_errors.hero` ≈ +12 under the EXISTING code `cannot_infer`), **0 in `selfhost/ir/` and `selfhost/emit/`**; B ≈ 45–60 plus the ~8-line call-site diagnostic B does not remove the need for. Two corrections to the brief: `check`'s `.method` arm never receives `expected` (UFCS is a third failing spelling), and `x: i64? = wanted(...)` fails today too (134). `--dump-ir \| grep -c '??'` is **0 over all 134** hole-free corpus files, so `no_error_survives` under `c.holes.len() == 0` refuses no legal program — and WITHOUT the condition it would turn a statement-position `???` into exit 2 (measured) | A: landing diff ≤100 added non-test lines in the three checker files, no line under `ir/`/`emit/` beyond the two repairs; the nine-position table scores 5 × `cannot_infer` and 4 × exit 0; at M-corpus-depth close `value.hero` is back to one generic and `??` over the corpus is still 0. B: the reproducer is exit 1 on line 7 and at least one more N-copy workaround appears in the five remaining programs | (1) `.method` routed like `.call`, golden for `return "a".wanted()` and `return wanted(what: "a")`; (2) context fills only UNBOUND parameters and never reports — one diagnostic for a disagreeing annotation; (3) the diagnostic is pushed where `walk.hero:1511-1515` enumerates the unbound, so no path leaves `user_call` silent; (4) the enclosing-generic context shape (`outer<B>` returning `wanted`) is a run golden; (5) `mono.hero:261`'s false comment corrected. Flips to VETO if A touches `mono.apply`, `phases` or `ctype`'s switch for itself |
| **spec-warden** | measured token counts, the counter, design.md | **A: OBJECT · B: APPROVE-WITH-CONDITIONS** (no veto: 3750 clears 4096, headroom 346) | §1.0 (burden), §1.2, §1.4, §1.6, Part 6 preamble | re-measured 3718 / 3750 / 3735 exactly; a ladder of cheaper wordings (A3 +13, A4 +20, B1 +3, B3 +15); **52 generic signatures across `selfhost/`, `examples/`, `tests/`, 52 of 52 with every parameter in a parameter's type** — panel 029 R7's letter CONFIRMED; **and a shape nobody had named: `function unused<A>(what: str) -> str`, `A` appearing NOWHERE, compiles at exit 0 today** — B refuses it, A's sentence never reaches an uncalled declaration; no §1.4 removal exists to fund either option | A: through M-corpus-depth's five remaining programs, 0 return-only generics are written (signature grep at close) — at 0 the tokens bought a rule nothing uses. B: the new diagnostic fires on zero repository programs other than its goldens, and the five remaining programs contain at most one refused return-only generic; falsified at two | for either option: `SPEC_TOKENS` and the ledger row in the same commit; the two owed repairs landed; design.md §4.12 amended beside *"look at argument types"*; `value.hero`'s comment says the shape is now by rule, not lost to a defect. For A specifically: a measured Part 11 effect or a closure-list program, **and the lowering repaired before the sentence lands — never the sentence first** |
| **ffi-pragmatist** | the proposal + the C a binding would need, compiled | **A: APPROVE-WITH-CONDITIONS (3, 4, 5 blocking) · B: APPROVE** (veto not exercised: neither option touches the ABI — a generic can never be `extern`, measured: `ffi_type` at exit 1) | §1.11, §4.19, §1.12, panel 029 R4b/R4c | a 15-line sqlite3 binding in both shapes: B 63 lines / 819 tokens, A 57 / 753 — **B costs 2 lines, ~22 tokens per typed reader**, the same per-type duplication a C author pays; **21 adjacent shapes measured today: 0 of 21 refused by `check`, 3 caught by the verifier, 3 caught by the element gate by accident, 12 abort at build with `assert failed: false`, and 3 BUILD CLEAN AND PANIC AT RUN TIME** (`-> [A]`, `-> {str: A}`: one instantiation `empty_of<?>` with `hero_unreachable()` in its body); panel 029 R4b's *"at the call site naming the inferred argument"* never shipped — `gate.hero:47-59`'s `note()` dedups on text and keeps the EARLIEST span, so the reader is sent to the call or the declaration by source order | A: `--dump-ir` on the two-type `empty_of` shows two instantiations and the binary prints `7 1 1 true`; `readers_A` prints exactly what `readers_B` prints, `--sanitize` clean; `s04`/`s05`/`s11`/`s14` exit 1 at the call — if any `_3f` (the mangled `?`) survives in `--emit-c`, A shipped R4c's silent-descriptor class. B: `check` exits 1 on all 21 probes at the `function ...<A>` line | (1) both: the three run-time panics become compile-time outcomes, and an error type reaching the gate or the mangler is `internal error:` naming the function; (2) both: the two owed repairs; (3) **A, blocking**: instantiations keyed by the context-bound type, descriptors re-derived from the substituted id (R4c), `A := str` and a store into a live `@` slot leak-clean under `hero_runtime_check_leaks` and `--sanitize`; (4) **A, blocking**: a boundary type bound from context names its source in the gate's message; (5) **A, blocking**: a no-context call is exit 1 AT THE CALL, naming the parameter and where a type could have come from, spec sentence in the same commit |
| **historian** (advisory) | the defect note and panel 029, web search for everything else | **A: APPROVE-WITH-CONDITIONS, flat only · B: APPROVE as the verifier's floor, OBJECT as the whole story** | precedent, every claim sourced in the sitting | **the mainstream of the last fifteen years took A**: Java (assignment context 2004, argument context 2014, JEP 101), TypeScript 2.4 (2017), Swift (bidirectional AND no call-site syntax — `f<T>()` refused in 2016 and again in 2024), Kotlin, Rust, Haskell, OCaml. **The refusers are the C-emitting, monomorphising lineage** — C++, D, Go, C#, Nim, Zig, Odin, Carbon — **and every one of them ships an outlet** (an explicit type argument or a type-valued parameter); no language was found that refuses AND has no outlet. Go's stated objection (Griesemer, #50285, 2023-05-16) is to the NESTED case `f(f(f(0)))`, and his own word for the localizable case is *"flat"*. C#'s stated blocker (overload resolution) does not transfer: Heroes has no overloading. Swift refuses at the declaration only a parameter that appears NOWHERE in the signature | A: over the next corpus milestone at least one more program writes a result-only generic and none needs the nested case; every no-context call is exit 1 at the call; no `??` reaches `--dump-ir` at exit 0. B: the shape recurs ≥1 per corpus milestone, each time as per-type duplicates | A → OBJECT if the ruling admits nesting through a generic callee's own type parameter; no diagnostic ever suggests a type-argument syntax (Rust's `::<_>` fixed point is the anti-pattern); the error names the parameter and the two ways to give it context. Panel 029 R7 scored **partial**: letter confirmed (the warden's grep), spirit refuted by one natural witness, remedy (*"GHC 8.0's answer"*, visible type application) refuted — both options refuse a turbofish |

## Disagreements, stated plainly

**The warden against three seats, on Principle 0.** The warden's objection to A
is the burden of proof: 52 of 52 signatures bind every parameter from the
arguments, the one witness cost two lines to rewrite, and one anecdote is not
§1.0's measured Part 11 effect. The ergonomist answers with the other branch of
§1.0, *"a §1-derived argument the panel accepts"*: in a language with no
constraints a result-only parameter can only produce a failure, an empty
container or a hole, so **whatever type the context picks the value is the
same** — a wrong guess can never compile into a different program, and B forbids
exactly the one class where context inference is provably safe, while the spec's
own `fail`, `nullptr`, `[]` and `{}` already take their type from context. The
engineer adds Part 6's own rule: a refusal must name the program or the compiler
fact that would make it wrong, and **B's premise — a parameter *"no call could
ever bind"* — is false**, because `check(expected)` binds it at every ⇐ position
and the reproducer is that program. The historian adds that every refusing
language gives the programmer a way to say the type, and Heroes by design has
none. The synthesis sides with the three, and records that the warden's cheaper
ladder (+13/+20) is available if the author wants the tokens back.

**The pragmatist's B-approve is not a vote against A.** The seat approved both,
priced B's cost honestly (two lines per typed reader) and made A's approval
conditional on three blocking items that are engineering, not design: distinct
instantiations per context-bound type, descriptors re-derived, and leak-clean
`A := str`. Those are the conditions under which the seat says A is sound, and
the resolution takes all three.

**"Flat" is not a restriction on A; it is what A is.** The historian's condition
and Go's objection concern `f(g())` where `g`'s result-only parameter would have
to be bound THROUGH `f`'s own type parameter. In this checker a generic callee's
arguments are synthesised, not checked against an expectation (`walk.hero:1500`,
measured by the engineer: `pair(a: ok(1), b: ...)` is already `cannot_infer` on
`ok(1)`), so the nested case is refused by the existing frontier and A adds no
unification variable — it hands the expected type the checker already holds past
the one line that drops it (`walk.hero:491`).

## Corrections to the brief, which the coordinator wrote

- **The brief cited "§1.5 (errors local)"**: §1.5 is the declaration/use
  asymmetry; locality is §1.3 and §4.5. The engineer caught it.
- **The brief's frontier implied `x: i64? = wanted(...)` works today.** It does
  not (exit 134); every ⇐ position drops `expected` at the `.call` arm's fall to
  `synth`.
- **The brief named two spellings; there are three.** `check`'s `.method` arm
  never receives `expected` (`walk.hero:533`), so UFCS is its own failing shape.
- **The brief missed the shape the warden found**: a type parameter that appears
  NOWHERE in the signature compiles today. Both options must refuse it, and the
  resolution does, at the declaration, with Swift's rule as the precedent.
- **The brief believed panel 029 R4b had shipped as written.** The pragmatist
  measured that the gate's message names the container and its span is decided
  by source order; the resolution's condition on the gate is therefore wider
  than the brief asked.

## Resolution — provisional, author ratification pending

**Option A, flat, is adopted**, as the most conservative resolution *among those
the panel accepts*: B was objected to by three seats, one of them on the ground
that its refusal has no falsifier, and A by one seat on a cost burden the
author's instruction of the same evening explicitly overrides. What a veto of
this resolution would compel: Option B in the warden's B3 wording (+15), the
declaration diagnostic, and `examples/interpreter/run/value.hero` keeping its
four duplicated lines with a comment saying the shape is refused by rule.

Under A the compiler does, in this order:

1. **The checker binds from the context.** After binding from the arguments,
   any type parameter still unbound is bound against the type the context asks
   for — the annotation of the binding, the enclosing function's declared
   result reaching a `return`, a `match` or `if` arm's expected type, a
   non-generic callee's parameter type, a record field's type. Context fills
   only UNBOUND parameters and never reports a mismatch itself (the existing
   `compare` does, so one mistake is one message). UFCS receives the same
   expectation as a call. A generic callee's arguments stay synthesised, so
   nothing is bound THROUGH another generic: the nested case Go declined is
   refused by construction.
2. **A call that says neither is refused at the call**, under the existing code
   `cannot_infer` (the shape `ok(...)`/`fail(...)` without context already has,
   `contextless_errors.hero`), naming the parameter, saying it appears only in
   the result, and offering the two ways to give it context as a `guess` fix —
   annotate the binding, or declare the enclosing result. **No diagnostic ever
   suggests a type argument**; the language has no such syntax.
3. **A type parameter that appears nowhere in the signature is refused at the
   declaration**, because every call to it would be case 2 and the declaration
   is the one line that can be fixed (Swift: *"Generic parameter 'T' is not
   used in function signature"*).
4. **Instantiations are keyed by the bound types wherever they came from**, so
   `empty_of` bound to `i64` and to `f64` is two copies with two descriptors, and
   nothing bound at the error id ever reaches the mono pass, the gate or the
   mangler.
5. **The two ruling-independent repairs** are in: `no_error_survives` in the
   verifier's mono and owned phases, under `c.holes.len() == 0`, naming the
   function and the slot at exit 2; and `ctype.hero`'s two `assert false` carry
   a message naming which verifier check should have refused the program first.
6. **The gate's message names the source** when a `ptr`/`cstr`/`()` element
   type was bound from an annotation or a result rather than from an argument.
7. **The spec gains the sentence above**, `SPEC_TOKENS` moves 3718 → 3750 in the
   same commit with ledger row 50, paid by the registered predictions below;
   design.md §4.12 gains the context clause beside *"look at argument types"*;
   `mono.hero:261`'s comment is corrected; `examples/interpreter/run/value.hero`
   returns to the one generic it was written with.
8. **Goldens for every shape the seats measured**: the reproducer and its
   `return`, UFCS and annotated spellings as run cases printing `7` and `true`;
   `-> [A]`, `-> {str: A}`, two types from one `empty_of`, `A := str`, a store
   into a live `@` slot, the enclosing-generic context, all run under the three
   configurations with the leak counter; the four no-context spellings and the
   nowhere-parameter as check cases with `#~ cannot_infer` / the declaration
   code; a verifier unit test that fires `no_error_survives`.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| llm-ergonomist | in a harness task with ≥3 fallible accessors over one variant, ≥30% of first tries declare a result-only helper, ≥90% of those compile, the no-context error fires in ≤5%; falsified at ≥20% | M-thesis-harness (the Part 11 instrument), when it runs |
| compiler-engineer | the landing diff adds ≤100 non-test lines across `walk.hero`, `generics.hero`, `contextless_errors.hero` and touches no line under `selfhost/ir/` or `selfhost/emit/` beyond the two repairs; the nine positions score 5 × `cannot_infer`, 4 × exit 0 | the landing commit |
| compiler-engineer | `value.hero` is back to one generic and `--dump-ir \| grep -c '??'` over the run corpus and `examples/` is 0 | M-corpus-depth close |
| spec-warden | through M-corpus-depth's five remaining programs, 0 return-only generics are written; at 0 the tokens bought a rule nothing uses | M-corpus-depth close (signature grep) |
| ffi-pragmatist | the two-type `empty_of` shows two instantiations in `--dump-ir` and prints `7 1 1 true`; no `_3f` in `--emit-c`; `s04`/`s05`/`s11`/`s14` exit 1 at the call | the landing commit |
| historian | over the next corpus milestone at least one more program writes a result-only generic and none needs the nested case; every no-context call is exit 1 at the call; no `??` reaches `--dump-ir` at exit 0 | M-corpus-depth close |

## Author's verdict

**Ratified 2026-09-03**, the same evening, minutes after the synthesis was
written (author instruction, *"Va bene, la ratifico subito"*, after asking and
being told in plain words that A is both the most robust and the safest of the
two: the natural program compiles wherever the type is known, a wrong guess
cannot compile into a different program, and every unresolved type is refused by
the verifier before the emitter). **What the yes settles**: Option A, flat, in
its complete form under every condition of the approving seats — the `.method`
arm routed, fill-only context binding, the diagnostic at the call under
`cannot_infer`, the nowhere-parameter refused at the declaration, instantiations
keyed by the bound types with descriptors re-derived, the gate naming the source,
the two ruling-independent repairs, the spec sentence with its ledger row.
**What it does not settle**: the six predictions, scored at their named moments;
the warden's cheaper wordings stay on the record as the author's option, not
taken.
