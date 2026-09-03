# Panel 029 — generics by monomorphisation, and the pass that had already decided

**Convened** 2026-08-11, on M6 step 6. **Trigger** architecture (IR) plus a
diagnostic class (CLAUDE.md §4). **Status** `provisional — author ratification
pending`.

## The proposal, verbatim

> M6 step 6, generics by monomorphisation. design.md §4.12: generics on
> functions only, no constraints, always inferred, never written at the call
> site. The body **lowers polymorphically** — `Ty::Generic(i)` survives into the
> IR and monomorphisation is an IR→IR pass at M6 (`ir/mod.rs:150`,
> `docs/ROADMAP.md:260`). Today `emit/gate.rs` refuses `Ty::Generic` and any
> function with a non-empty `generics` list. **My claim, which no judge should
> take: this step costs zero spec tokens.**

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **object** (would veto one plan) | the ownership pass has **already decided, wrongly**: it committed `is_refcounted(Ty::Generic) == false` into bodies that will be instantiated at `str` |
| llm-ergonomist | **veto**, scoped to `fold` as specified | four well-typed programs, one source text, four outputs, zero diagnostics — and the language's own defence against argument-order mistakes **cannot fire** at a higher-order call site |
| spec-warden | **object** — the zero-cost claim is **false** | priced the counterfactual the M6 audit needs: striking generics and the six higher-order names is **−83**, and it says *don't* |
| historian (advisory) | approve | **MLton** has monomorphised whole programs for twenty-five years and terminates *because* SML bans polymorphic recursion |
| ffi-pragmatist | **object** | found a **live defect**: `[()]`, `[ptr]` and `{str: ptr}` check clean, build, and abort with *"this is a compiler bug"* — and step 6 makes it reachable by **inference** |

## The finding that reorganised the panel

**The convener's zero-cost claim is false, and two judges falsified it from
opposite ends without seeing each other.**

The ergonomist, reading only the spec, built this:

```
function cat(acc: str, w: str) -> str
    return acc + w
...
    print(words.fold("", cat))
```

Callback `(accumulator, element)` gives `alphabbcccdd`; `(element, accumulator)`
gives `ddcccbbalpha`; a right fold gives two more. **Four well-typed programs,
one source text, four outputs, zero diagnostics.** The warden reached the same
place from the compiler and named the line: `types/builtins.rs:263` requires
`params[0] == start && params[1] == element`, which **both** orders satisfy when
the accumulator and the element share a type. The convener verified it a third
time: `function cat(w: str, acc: str)` — the swapped order — checks clean at
exit 0.

`sum`, `concat`, `max`, `min` and every string builder are same-typed folds. This
is not an exotic corner; it is most real folds.

**And the structural half, which is the ergonomist's alone.** The spec contains a
rule whose entire purpose is preventing argument-order mistakes — *"when two
parameters in a signature share a type, named arguments are mandatory at the call
site"*. At a higher-order call site **there is no call site in the author's
file**: `fold` calls `cat`, and `fold` is not in their file. The protection is
switched off exactly where the hazard lives, and the parameter names that carry
the author's intent are discarded.

**Why this belongs to *this* step and not to a later one.** `fold` is refused by
the gate today, so the divergence has never been observable. **Landing generics
is what makes it reachable.** The step carries the fix or it ships the defect.

## Resolution — provisional, author ratification pending

### R1 · Monomorphisation runs **before** the ownership pass

Not because ownership "cannot decide" with generic types. Because it **has
already decided, wrongly** — the engineer dumped it and the convener reproduced
it:

```
function combine<A, B>(xs: [A], start: B, f: (function(B, A) -> B)) -> B
    bb0  entry
        $t1: B = load start
        store total <- $t1          <- no incref, no decref
        $t2: [A] = load xs
        incref $t2                  <- the array is counted; B is not
```

`is_refcounted` answers `false` for `Ty::Generic` (`types/counted.rs:89`), which
is right for `B = int` and a leak for `B = str`. `ir/phases.rs` cannot catch it:
`released_on_return` asks the same lying predicate. So the order is forced, and
the reason is stronger than the ROADMAP's.

### R2 · The substitution is **recorded in `Checked`**, never recomputed

`types/calls.rs:260` computes `bindings`, uses them, and drops them; `Checked`
has no table; `Op::Call` carries no type arguments. Recomputing them inside the
pass would duplicate `types/generics.rs::bind` at IR level — and
`types/counted.rs:11-15` already ruled on that shape by name: *"Two answers that
disagree … is a refcount bug that reproduces once a week."* The engineer's stated
veto condition is a plan that recomputes. A dense side table plus a `TyArgs` pool
mirroring `Args`/`Steps` is therefore **forced, not chosen**.

### R3 · `Checked::counted` is extended after the pass, with a golden that proves it

`types/mod.rs:198-203` says `counted` is built last *"because nothing may intern
a type after it: a `TyId` past the end of this table would read as uncounted,
which is a leak that no test can see."* Monomorphisation interns `[str]` where
only `[A]` existed. `ir/layout.rs:87` is a bare index whose doc says an
out-of-range `TyId` is a compiler bug and panics.

Unhandled it panics; softened to a bounds check it is exactly the silent leak
that comment describes. The step lands `counted` extended after the pass **and** a
golden instantiating one generic at `str` and at `int` in the same program, run
under `--sanitize` with the leak counter green.

### R4 · Polymorphic recursion is **refused by the pass**, and MLton is why that is free

`f<T>` calling `f<[T]>` instantiates forever. Verified: it **type-checks clean at
exit 0 today**, and design.md does not mention it anywhere — the engineer checked
design.md, the ROADMAP and the spec and declined to invent a rationale.

The historian supplied the one that exists. Inference for polymorphic recursion is
undecidable (Henglein, TOPLAS 15(2), 1993; Kfoury–Tiuryn–Urzyczyn, TOPLAS
15(2):290–311, 1993 — reduction to semi-unification), which is why Haskell and
OCaml permit it only with an explicit polymorphic signature. **MLton's
Monomorphise pass is whole-program and total precisely because SML bans it**:
*"due to the absence of polymorphic recursion in SML, there are in fact only a
finite number of instances of such types in any given program."* Twenty-five
years of exactly this design.

The counter-example is Rust, and it is a warning rather than a model: it does not
reject at type-check but blows up at codegen with `reached the recursion limit
while instantiating` — **no error code**, late, unattributable. That is what this
project's thesis refuses.

**Where the refusal lives.** Not the checker: it checks a generic body **once,
polymorphically**, and never expands, so it cannot see the growth. Not a depth
limit: that exits 2 — *"the compiler could not run"* — for a program §4.12 makes
legal. So the pass detects it structurally: instantiating `f` at `S` from within
`f` at `T` where **`S` properly contains `T`** is unbounded, decidable, and
exactly the finiteness argument MLton rests on. It is a **program** diagnostic,
exit 1, naming the function and both instantiations.

### R4b · The gate descends into element types, and a filed defect comes with it

The ffi-pragmatist went looking for a concrete type C cannot spell and found one
that needs no generics at all. Verified by the convener, and broader than
reported — three container kinds, not one:

```
$ heroes check unitarr.hero      # xs: [()] @ []
exit 0
$ heroes run unitarr.hero
panic: entered unreachable code — this is a compiler bug, please report it
```

`[ptr]` and `{str: ptr}` do the same. The cause is `emit/gate.rs:146`:
`Ty::Array(_) => return` **does not descend into the element type**, so the
`ffi_type` row at line 161 never fires and `Ty::Unit` at 144 returns
unconditionally. The emitted C says so itself: `hero_unreachable(); /* not an
array */`.

**Today you must type `[ptr]` to reach it. After step 6 you infer it** —
`map(nums, print)` infers `B := ()`, `map(handles, close)` infers `A := ptr`, and
the author never writes the type that lands here. So the step owns the fix: the
gate descends into `Ty::Array`, `Ty::Map` and `Ty::Fallible`, and the pass
refuses `()`, `ptr` and `cstr` as type arguments with a diagnostic **at the call
site naming the inferred argument**. It earns a case named after it (CLAUDE.md
§9).

### R4c · The descriptor is re-derived from the substituted `TyId`, never carried

An instantiation that substitutes the type but keeps the old descriptor is
**invisible to every instrument this project has**. Measured: `hero_array_new(&hero_desc_int, 1)`
where `B` is `f64` compiles at exit 0, runs at exit 0, is silent under ASan and
UBSan, leaves the leak counter at zero — and prints `[-0.0] == [0.0]` as `false`
where Heroes says `true`. `sizeof(int64_t) == sizeof(double)`, both copy by
assignment and drop nothing, so only `eq` and `hash` change and the memory stays
well-formed.

So the descriptor comes from `descriptors::pointer` applied to the **substituted**
`TyId` — never carried through substitution as a string — with an IR-verifier
check and a `heroes mutate` invariant over the corpus. The judge's `silent.c` is
the case that must fire.

### R5 · The mangled suffix hashes the **canonical rendered type**, never the `TyId` sequence

CLAUDE.md §7 already prescribes `h_<module>_<name>[_<typehash>]`. Two judges
constrain what goes into it, and they disagree until the constraint is seen.

The historian: RFC 2603 lists four deficiencies of Rust's legacy hashed mangling,
and the hash is not the first — *"one cannot extract the type arguments of a
monomorphized function from its symbol name"* and *"it depends on compiler
internals and its results cannot be replicated by another compiler
implementation"*. Rust's v0 scheme took **eight years** to become the default
(stable in 1.97.0, 2026-07-09). Itanium C++ has been structural and hashless
since 1999.

The engineer: a structural suffix is **not spellable**. `render_ty`'s output
contains `[ ] { } ( ) ? ,` and spaces, none legal in a C identifier, so it needs a
second escape mangler, and nesting is unbounded.

**The resolution takes both.** The suffix is a hash **of the canonical rendered
type string** — which answers RFC 2603's deficiency (iv) exactly, because another
implementation reproduces it from the same public rendering. It is emphatically
**not** a hash of the `TyId` sequence: `TyId` is an interning-order artifact, and
the M8c fixpoint would then require the Rust bootstrap and the Heroes port to
intern in identical order. `mangle.rs:29-33` already applies that same reasoning
to `module_of`.

Deficiency (i) — information loss — is answered for **free** rather than
accepted: each instance is emitted under a comment naming its types,
`/* map<int, str> */`. It costs nothing at link and restores what a reader of
`--emit-c` needs.

**And the ffi-pragmatist closed the question by compiling both.** A readable
suffix is **not injective**: with `record int_str`, `record str_x` and `record x`
in one file, `pair<int, str_x>` and `pair<int_str, x>` both spell
`h_m_pair_int_str_x`, and clang answers `error: conflicting types` **on a legal
Heroes program**. A false rejection is worse than an opaque name, and it is
`mangle.rs`'s own injectivity argument one level down. It also measured that the
readable form buys nothing where it would matter most: clang prints the `aka`
expansion and the `note: passing argument to parameter 'f' here` either way, so a
*type* error already carries the full instantiated signature. The two places a
name is all you get — `ld: duplicate symbol` and an ASan frame — are answered by
`--dump-ir` printing the instance table, which is an existing invocation and
therefore nothing under CLAUDE.md §10.

### R6 · Spec: `fold`'s accumulator order, **+19 measured** (2354 → 2373)

Inline in the function-values bullet, the cheapest of four forms the warden
measured: *"— accumulator first, so `add` is `(function(B, A) -> B)`"*. A
standalone sentence is +20, a type-asymmetric example +32, the full six-signature
table +67.

Panel 025 R3's three teeth, applied by their author: it cites **no** design.md
mandate, so it takes full burden under §1.0's second limb and rides panel 012's
pre-registered prediction; it **decides nothing**, because design.md:2415 already
fixes `reduce<A, B>(xs, initial, (function(B, A) -> B))` and the compiler
implements it — *the spec is behind design.md, not choosing*; and the warden
verified on every type the sentence covers, which is what found the defect.

`find -> A?` is measured at a further **+8** and **not funded**: guessing it as an
index fails loudly, and panel 025's ranked rule spends nothing where the compiler
already speaks.

### R7 · Part 5's closing sentence is amended, because this row falsifies it

design.md:1944 closes the sugar table with *"Everything in this table is gone **in
the IR**: lowering erases it on the way in"* — while line 1941 of the **same
table** says generics are erased *"on the IR, after type checking"*. One row
erased by a pass rather than by lowering is fine; a table that claims otherwise is
not. `ir/verify.rs:27-30` already documents the exception informally.

### R8 · `#0` is nobody's type, and the step must not step over it

A generic function used as a value reports `expected `#0`, found `int`` —
`Ty::Generic(0)` printed raw to a reader. `types/calls.rs:219-229` fixed exactly
this defect on the label path and says why in those words; the `FuncRef`-as-value
path was never fixed, and §4.13 is silent on generic functions as values. **No
spec tokens** — Principle 0 says the compiler does not need it — but the pass
asserts no `Ty::Generic` survives, so this sits directly on its path.

### R5b · Panel 028 R3b does not cover an instance, and the amendment is M8a's

The ffi-pragmatist tried to apply R3b — *"one definition in the library's own
translation unit, many prototypes"* — and it does not compile:

```
tu2/library.c:3:34: error: a parameter list without types is only allowed in a function definition
tu2/library.c:7:25: error: use of undeclared identifier 'h_a_Point'
```

The library translation unit does not know the user's types, and **instantiation
is caller-driven**. So an instance cannot live where `range` lives, and the
library cannot be compiled-once-and-cached the way `runtime.o` is.

Its measurements of the three alternatives, at all three optimisation levels:

| | −O0 | −O2 | −O2 −flto |
|---|---|---|---|
| external (R3b's shape) | `ld: duplicate symbol` | same | same |
| `static` per TU | links — but `h_a_addr() == h_b_addr()` is **false** | same | same |
| `weak` | links, identity **preserved** | same | same |

The `static` cost is new and it is step 5's doing: a function value is now a bare
C pointer whose `eq` is pointer identity, so two modules holding "the same"
instance would compare unequal.

**This panel records the finding and does not decide it.** Panel 028 vetoed
`weak` on its own measurement — two units, one symbol, *different bodies*, zero
diagnostics, wrong answer — and that veto is not overturned by a judge that wants
`weak` for a different reason. Nothing forces the choice at M6, which has one
translation unit; M8a is where it binds, and it will bind with both measurements
in the record rather than under deadline. R3b is amended to say **it does not
cover instances**, which is the honest version of "amended, not applied".

### R5c · Generics close panel 027's open question, and that is the strongest argument for the step

Panel 027 vetoed a `cmp` in `HeroDesc`, and §4.20's closing line says the
replacement is *"passed as a parameter (§4.12)"*. The ffi-pragmatist built it:
a generic sort monomorphised at `T = str` and at a user record with a `str` field
— correct output, ASan and UBSan clean, leak counter zero, and **no `qsort`, no C
callback, no shim, no sixth descriptor field, no new runtime entry point**.

It also demonstrated the claim it argued at panel 027 rather than repeating it: a
Heroes `-> int` comparator whose low 32 bits are zero makes `qsort` read every
pair as equal — `{5,1,4,2,3}` prints unsorted, clang silent under this project's
flags. The boundary holds anyway, and it verified *why*: the checker refuses the
launder (`expected `ptr`, found `(function(ptr, ptr) -> int)``), so what a Heroes
program must write for a C callback is a shim, which is exactly §4.19's
prescription.

**And the thesis survives monomorphisation, measured.** `map(xs, sqrt)` against
the real `<math.h>` puts the extern's bare address into the instance's parameter
with no cast and no shim; a wrong `extern function sqrt(x: int) -> int` reaching
the same instance is `error: incompatible function pointer types passing 'double
(double)' to parameter of type 'h_m_fn_int'` — an error **without** `-Wall`.

## What a veto would have compelled, and the number the audit now has

The warden priced the counterfactual: **striking `map`/`filter`/`fold`/`find`/
`any`/`all` and the generics bullet is 2354 → 2271, −83** (the generics bullet
alone −52, the six names −31). Its answer is *don't*, and not from sentiment:
`generics on functions` is on §1.0's closure list **in its own right**
(design.md:138), and §4.12's finding is that without them the six become "seven
special cases in the type checker, each with hand-written rules" — which is
literally `types/builtins.rs:166-181` today. The −83 buys permanent compiler
magic. **The M6 audit now has its number.**

## Two acts of judicial integrity, recorded because they are rare

**The warden refused its own removal.** It measured one (−15: collapsing the
three function-type arities to one plus "any arity") and then declined it,
because panel 013 bought those three forms for +14 *as a condition of approval*
against a measured ≥25% arity-error rate. It reports instead that this step rides
panel 012's second route again, extending design.md:290's named failure mode
("zero removals in seven amendments") to eleven.

**And it scored its own prediction refuted without an escape hatch.** Panel 027
#9 — *"spec ≤ 2350 at M6 close"* — is **refuted at 2354**, and it ruled that panel
028's repair of a pre-existing self-contradiction **counts against it**: *"the
prediction was on a number, not on a set of reasons; exempting repairs of
pre-existing defects gives every overrun an escape hatch."* Its stated lesson:
it priced a budget without provisioning for the spec's own latent contradictions.

## Predictions to score

| # | judge | prediction | checkable |
|---|---|---|---|
| 1 | compiler-engineer | the step's net non-test Rust exceeds **+220 lines across ≥10 files** while `ir/mono.rs` itself is **≤250** — plumbing outweighs the pass; `grep -c "Ty::Generic"` rises from **9** to **≥13** and `types/counted.rs` grows from **125**. If counted.rs is unchanged, objection 1 was not addressed | M6 close |
| 2 | llm-ergonomist | **≥40%** of first-try `fold` callbacks are written in the opposite order; of those, **≥95%** are compile errors where the types differ and **≥90% compile and print a different program** where they coincide | next harness run |
| 3 | llm-ergonomist | first-try success orders task 2 > task 3 ≈ task 1 > task 4, and **task 4's failures are loud while task 1's are silent** — the inverse of their difficulty. That inversion is the finding | next harness run |
| 4 | llm-ergonomist | **≥80%** of higher-order attempts declare a wrapper rather than passing a built-in as a value, because whether that is legal is undecided | next harness run |
| 5 | spec-warden | among generated programs folding at a same type, **≥20% pass the function element-first and 100% of those exit 0** with a wrong answer; the +19 sentence drops it **below 8%** | next harness run |
| 6 | spec-warden | spec **≤ 2373 measured** at M6 close | M6 close |
| 7 | historian | **every** generic in the self-hosted compiler has all type parameters determined by parameter types alone; zero need a return-position-only parameter. If even one does, inference-only breaks and this project reaches for GHC 8.0's answer | M8 |
| 8 | ffi-pragmatist | §4.19's ladder step 2 needs **no shim** under this rule: `map(xs, sqrt)` compiles, links and runs with the extern's bare address reaching a `double (*)(double)` parameter, and a wrong `extern` signature is a clang error naming both types. Falsified by any marshalling, thunk or `_Generic` appearing between an extern's address and a monomorphised parameter | M7 |
| 9 | ffi-pragmatist | **rider, checkable without `extern`**: the first corpus program instantiating a generic at `()` or `ptr` builds a binary that aborts `panic: entered unreachable code` rather than producing a diagnostic — unless step 6 gates the type-argument domain | M6 close |

**Scored here.** Panel 027 #9 **refuted** (above). Panel 028 #6 — *"deleting `enum
Tier` and the `tier:` field leaves `cargo test` green with zero golden `.expected`
touched"* — is **CONFIRMED by experiment**: the warden ran it, 364 + 13 + 25 green,
`git status` showing only the two `.rs` files it edited, then reverted. The `tier`
field is read **nowhere** outside its own declaration and one re-export. Panel 026
#6 (< 2450 through 2026-09-30) is alive with 77 tokens of headroom after R6.

## Watch list

- **The convener's framing was wrong twice.** `emit/decls.rs` is **310** lines, not
  311; and §11's ceiling is not about to be breached, it is breached **five times
  over** — `perfn.rs` 589, `aggregate.rs` 511, `fmt_stmt.rs` 381, `inst.rs` 360,
  `calls.rs` 343. The compiler is 22 320 non-test lines.
- **`types/builtins.rs` loses ~90 lines** when the six higher-order rules become
  library source — `Shape`, `higher_order`, `fold` and six arms. §4.12's claim
  that generics make the compiler *smaller* is real but partial: it does not pay
  for the pass.
- **The historian corrected the convener three times**: Rust's polymorphisation
  was **removed** in 1.85.0 (2025-02-20), not paused; Swift compiles generic
  bodies against **type metadata** plus protocol witness tables, not value witness
  tables, and reabstraction is a different mechanism; Itanium mangling is
  structural but not fully reversible.
- **A user generic instantiated in two modules has no named home.** Panel 028 R3b
  named one for *library* generics ("one definition, many prototypes, in the
  library's own translation unit") and `static` was refused there on
  `-Wunused-function` noise. Residual, not blocking: M6 has one translation unit.
- **`_` as a label works**, contrary to the ergonomist's report: `keep_first(acc:
  a, _: b)` runs and prints. Its "declarable but not callable" finding is
  **withdrawn** — verified by the convener rather than accepted.
- **Four gaps the ergonomist named and this panel does not fund**: whether a
  built-in may be passed as a value (undecided, and it forces a wrapper per
  program); `join`'s argument shape; `print`'s rendering of `bool`; and the
  indentation of a continuation line inside `[`. All fail loudly except the third.

## DESIGN-LOG

Appended 2026-08-11 — see the lines citing panel 029.

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

## Prediction 7 scored at panel 105 (2026-09-03)

The historian's row — *every generic in the self-hosted compiler has all type
parameters determined by parameter types alone; zero need a return-position-only
parameter; if even one does, inference-only breaks and this project reaches for
GHC 8.0's answer* — scored in three parts by the seats of `docs/panel/105`:

- **Letter: confirmed.** The spec-warden read every generic signature in
  `selfhost/`, `examples/` and `tests/` — 52 — and 52 of 52 bind every type
  parameter from a parameter's type. (The compiler-engineer's grep over
  `selfhost/` alone found 0 generic functions; the warden's 15 there are the six
  library sources and nine test strings.)
- **Spirit: refuted, by one witness.** `examples/interpreter/run/value.hero` was
  written with `wanted<A>(what: str, v: Value) -> A?` because that is the natural
  spelling of one refusal shared by two result types, and rewritten as two
  functions to dodge the compiler. One instance refutes "zero"; it is not a
  trend.
- **Remedy: refuted.** The row said the project would reach for visible type
  application; the sitting refused a turbofish from both options and ruled for
  inference from the context instead — the answer Swift, Kotlin, Java 8 and
  TypeScript 2.4 took, none of which needed the syntax for this case.

And the row's premise was never *enforced*: inference-only was the compiler's
behaviour, not its rule — the checker accepted the program and the emitter died.
