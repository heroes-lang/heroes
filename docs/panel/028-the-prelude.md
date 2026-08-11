# Panel 028 — the prelude, and three sentences of the spec that cannot all be true

**Convened** 2026-08-11, on M6 step 4. **Trigger** architecture plus `spec/**`
(CLAUDE.md §4). **Status** `provisional — author ratification pending`.

## The proposal, verbatim

> M6 step 4, the prelude. Tier 2 of design.md §1.11 — `map`, `filter`, `fold`,
> `find`, `any`, `all`, `range` — is "written in Heroes itself" and arrives as
> source in one prelude. `range(a, b) -> [int]` needs neither generics nor
> function values, so the prelude lands with it. Four candidates for the
> mechanism: **P-source** (a `prelude.hero` beside `runtime/`, discovered and
> parsed per compilation), **P-embed** (the same source `include_str!`d into the
> compiler), **P-ir** (lowered once and cached), **P-c** (emitted as C once and
> linked like the runtime).

## The finding that reorganised the panel

**Three things the spec says are jointly impossible, and implementing `range`
is what made that visible.**

| | line | says |
|---|---|---|
| 1 | 113-114 | ranges are **`range(a, b)`** — positional, two arguments |
| 2 | 81-82 | "When two parameters in a signature **share a type**, named arguments are **mandatory** at the call site: `copy(from: a, to: b)`" |
| 3 | 151-152 | `range` is **written in Heroes** |

A built-in escapes rule 2 because `types/builtins.rs` checks it directly. A
Heroes *function* does not. Verified by the convener rather than taken from the
judge who found it:

```
$ heroes run upto.hero        # function upto(a: int, b: int) -> [int]
error[needs_label]: two of `upto`'s parameters are `int`, so every one of them
is named at the call site — this one is `a`
  10 |     for i in upto(0, 3)
     |                   ^
  fix (certain): write `a: `
```

So the moment `range` stops being compiler magic, `range(0, n)` — the form the
spec itself writes, twice — becomes a compile error. **The spec has contradicted
itself since v0 and nobody noticed, because `range` had never been
implemented.** design.md:1046 and design.md:1942 carry the same contradiction.

## The candidates, measured

Base **2345** (commit `9939d04`). Every figure re-measured by the warden, who
confirmed all three of the convener's and added four of its own.

| | wording | count | Δ |
|---|---|---|---|
| A | the spec as it stands | 2345 | +0 |
| r1 | "…`range(a, b) -> [int]`, `b` excluded." | 2350 | +5 |
| r2 | "…: `a`, `a+1`, … up to but not including `b`." | 2362 | +17 |
| r3 | "…, `b` excluded like `slice`'s `to`." | 2358 | +13 |
| x1 | the tier phrase "— and, written in Heroes:" **deleted** | 2339 | **−6** |
| x2 | r1 + x1, the warden's bundle | 2344 | −1 |
| **L1** | **"…`range(from: a, to: b) -> [int]`, `to` excluded."** | **2354** | **+9** |
| L3 | labels only, exclusivity unstated | 2349 | +4 |

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **object**, with a **veto on P-ir** | the step **as scoped cannot ship**: §4.11 fires on a declared `range(a, b)`, and all four corpus call sites are positional |
| llm-ergonomist | **approve** stating the bound; **veto** on leaving it silent | **five of six** off-by-one directions it tested are **silent**, and the one loud case survives on an accident of the task's shape |
| spec-warden | **object** — all three candidates correctly priced and all three the wrong shape | found the project's **first measured removal by asking what a sentence does**: the tier phrase, −6 |
| ffi-pragmatist | **object**, with a veto on `weak` and a veto on **P-c as the general mechanism** | at **M8a** two modules that both call `range` are `ld: duplicate symbol` at −O0, −O2 and −flto alike, because the emitter gives every Heroes function external linkage |
| historian (advisory) | approve | **generics travel as IR, never as code** — GHC and rustc converge, so no prelude can be fully precompiled; and the convener's sharpest precedent **does not bind** |

## Where they disagreed, stated rather than smoothed

**The tier phrase: the warden measured that it buys nothing, the ergonomist
reported that it buys three things.** Both are measurements, of different
objects. The warden checked the *compiler*: `Tier` appears nowhere outside the
table that defines it, and it verified that a Tier-1 name passes as a function
value exactly like a Tier-2 one, so the tier grants no privilege. The
ergonomist checked *itself*: the phrase told it `range` is a value rather than
syntax ("I never wondered whether to write `for i in 0..n`"), that it
allocates, and that these names obey the ordinary rules.

**The ergonomist's second reason is what decides it, and neither judge could
have seen why.** "They obey the ordinary rules" is exactly the sentence that
makes §4.11 apply to `range`. The tier phrase is not decorative: it is the
premise from which the labels below follow. Delete it and `range(from:, to:)`
looks arbitrary. The warden's own withdrawal condition — *"I withdraw the
removal if anyone shows a program a reader writes differently because of the
tier phrase"* — is met by the other judge's transcript.

The removal stays **pre-measured at −6 and available**, and it is the first one
this project has found by asking what a sentence *does*. That method survives
even though this particular sentence did.

## Resolution — provisional, author ratification pending

### R1 · `range` obeys §4.11: `range(from: a, to: b)`. Spec **L1, +9 measured**

The collision has four exits and three of them cost more than they save:

- **an exemption** for prelude declarations — a hole in what design.md:1184
  calls "the most original construct in the language", and panel 015 already
  rejected a prelude exemption once;
- **`range(n)`**, one parameter, 0-based — the engineer's own condition for
  approval, and it dodges §4.11 completely. Refused because the ergonomist,
  writing from the spec alone, produced `range(1, n + 1)` for the first task it
  was given. Under one parameter that shape has no spelling;
- **`range` stays a built-in** — the spec becomes consistent by making item 3
  false. That is the M6 closure-list audit's decision (Principle 0's checkpoint,
  panel 005's three riders), not a step's, and pre-empting it here would be
  exactly the sunk-cost move the ROADMAP warned about for `{K: V}`;
- **obey the rule**, which is what lands.

`range(from: 0, to: n)` needs no exemption, no tier change and no new rule. It
turns the universal habit — `range(0, n)`, which every Python, Go and Rust
reflex produces — into a **compile error carrying a certain fix**, one
machine-applicable round trip. That is §1.4 working, not failing.

And it is a **repair**: the spec's own example of rule 2 is `copy(from: a, to:
b)`, so `from:`/`to:` is the vocabulary the document already teaches for a pair
of same-typed bounds. `slice(from:, to:)` uses it too, and its `to` is excluded
— so `range`'s labels and `range`'s bound now answer each other.

Six call sites change: `examples/gallery/{03,04,07}`, and two under
`tests/golden/check/` where `UPDATE_GOLDEN=1` is forbidden and the edit is
therefore by hand, with the diff quoted.

### R2 · The bound is stated, and it is the ergonomist's strongest result

Under the silent spec it worked the six off-by-one directions through by hand
rather than asserting them:

| task | one too many | one too few |
|---|---|---|
| `sum_to(5)` | prints `21` — **silent** | prints `10` — **silent** |
| index every byte | `s[5]` aborts — loud | four lines, `"hello"` becomes `"hell"` — **silent** |
| first n squares | `5` / `16` — **silent** | `3` / `4` — **silent** |

Five of six are silent, and *"the one loud case is loud only by accident: it
survives because the loop bound happens to be derived from `len` of the thing
being indexed"* — change it to `range(0, len(s) - 1)`, which is what a hedging
model writes, and the abort disappears too.

It also found that silence here is not neutral but **misleading**: `slice`
carries "(`to` excluded)" three lines away, so omission reads as deliberate
contrast — *expressio unius* — and argues for the inclusive reading. And that
the real cost is not the wrong answer but the avoidance: under doubt it stopped
using the built-in and hand-rolled a `while` loop, *"three new places to be
wrong in exchange for one bound I can see"*.

### R3 · P-ir is vetoed; the prelude is source, compiled with the program

The engineer's veto is structural and the historian's precedent is the same
line drawn from outside. `TyId` is an index into a per-compilation interning
arena and `Ty::Named` indexes `Ast::decls`, so a spliced `Program` needs a
remap walk over 24 `Op` variants plus a serialisation format and a cache key —
**and it does not remove the merge**, because `emit()` still takes the `ast`,
`checked` and `src`. The merge is paid twice.

The historian supplies why no amount of engineering fixes that: **generics
travel as IR, never as code**. rustc's collector runs "just before MIR lowering
and codegen", so rlibs ship MIR for generics and instantiation happens in the
*user's* crate; GHC's `.hi` files carry unfoldings and cross-module
specialisation requires them. All six of the remaining Tier-2 functions are
generic, so the boundary is not a choice.

**P-c is vetoed on compiled evidence, not on the argument above.** It survives
`range` — the judge built `heroes_prelude.h`, compiled the library as its own
unit and linked it like the runtime, and it works. It dies at the first generic:
with the instantiation set unknown when a prebuilt library is compiled, the only
prebuildable `map` is **type-erased**, and passing a `str`-typed function to map
an `[int]` gave **zero diagnostics under `-Weverything`** and then `SEGV`
(`heap-buffer-overflow, READ of size 16` under ASan). `runtime/runtime.c`'s own
header already forbade it in one sentence — *"a type-erased `void*` runtime would
void the 'clang type-checks every call' property"* — and that sentence is now a
measurement. It also found the hazard P-c carries even for `range`: a header and
a separately generated unit that disagree compile, link, run and print
`6096514921` at exit 0, with the `_Static_assert` silent because nobody bumped
it.

P-source buys no capability
while opening a trust boundary the runtime does not have: CLAUDE.md §7's
`_Static_assert` exists so a decoy `runtime/` cannot replace the contract, and a
decoy `prelude.hero` has **no analog** — clang cannot type-check Heroes source.

**P-embed**, then: the prelude is `include_str!`'d, concatenated with the user's
file into one `Source`, and compiled with it. Panel 015 already recorded this
option verbatim — *"the index needs a unit tag or a documented concatenation"* —
so it is a decision being discharged, not invented.

### R3b · The linkage rule is decided **now**, though nothing implements it until M8a

The ffi-pragmatist's objection is that choosing P-embed today *defers* a rule
while making it unavoidable, and it compiled the proof. `emit/decls.rs::signature`
emits **no storage class**, so every Heroes function has external linkage. At
M8a — one `.c` per module — two modules that both call `range` produce:

```
duplicate symbol '_h_prelude_range2' in:
    dup_b-ec0804.o
    dup_a-8dd7ea.o
```

at −O0, −O2 and −flto alike, and `map<int, int>` instantiated in two modules
fails identically. It would surface as `internal error: compiling the generated
C`, **exit 2** — the compiler blaming itself for a language decision nobody made.

So the decision is made here, with the evidence, rather than at M8a under
pressure:

- **`__attribute__((weak))` is vetoed**, and the veto is a measurement. Two
  translation units, the same mangled symbol, **different bodies** — the shape a
  skewed per-module cache produces — link and run under this project's exact
  flags with **zero diagnostics, exit 0, and a wrong answer**: one module printed
  `4` where its own source says `6`. `-Weverything` reported only
  `-Wmissing-prototypes`. That is `toolchain.rs`'s own documented ghost promoted
  from a build accident to a language mechanism.
- **`static` everywhere is refused**: a module that carries a definition it does
  not call is `-Wunused-function`, which the judge projects at up to ~1200
  warnings across the port's modules — and by panel 022's own reasoning, that is
  how a real diagnostic gets lost in noise.
- **Adopted: one definition, many prototypes.** A library or monomorphised
  function is emitted into **exactly one deterministically chosen translation
  unit** — the library's own — and every other unit sees a prototype. E1f/E1g
  compiled and ran it at both levels.

### R4 · The word "prelude" is taken, and the emitter had it first

`emit/decls.rs:44` is `fn prelude(w, program, src)`, which writes the C header.
Two meanings of one word in one crate is a §11 readability defect. The library
source is **`library/`** in the tree and "the library" in prose; the emitter
keeps `prelude` for the C preamble it already names.

### R5 · A diagnostic inside library source is a compiler bug, and must say so

The historian's prediction is that the first defect here will be **diagnostic
provenance** — an error raised inside library source pointing at the library
rather than at the user's call site, which is CLAUDE.md §8's whole subject. So
the invariant is asserted rather than hoped for: **no diagnostic may carry a
span inside the library region**, and one that does is reported as an internal
error naming the library function, not as a user error.

### R6 · The tier phrase stays; the removal stays available at −6

See the disagreement above. Recorded so that the next panel does not rediscover
it: the method that found it — asking what a sentence *does* rather than what it
says — is the durable half.

### R7 · What is deliberately not decided

`range(5, 2)` — empty or abort — is left silent. The warden priced it at +6
bundled and refused it under its own tooth (b): design.md is silent, both
answers are plausible, and both fail **loudly**, so panel 025's ranked rule
spends nothing there. The ergonomist independently flagged it as the residual
neither variant closes. It is decided when the library's body is written and
not before.

## What a veto would have compelled

Had P-ir landed: a remap walk over 24 `Op` variants, a serialisation format, a
cache key, and the AST/`Checked`/`Source` merge **still** paid at emission —
300 to 430 lines to avoid 95 to 130.

Had the step shipped as scoped: `heroes check tests/golden/check/certain-fixes.hero`
exits 1 with `error[needs_label]`, and two goldens in the one directory where
regeneration is forbidden would have had to be hand-edited to accommodate a
contradiction rather than to record its repair.

## Predictions to score

| # | judge | prediction | checkable |
|---|---|---|---|
| 1 | compiler-engineer | P-embed's diff to `crates/heroes/src/` is **under 200 net non-test lines across ≤8 files**; over 300 means the concatenation was abandoned | M6 step 4 |
| 2 | compiler-engineer | `heroes check tests/golden/check/certain-fixes.hero` exits 0 **only if** the diff contains one of: fewer than two same-typed parameters, a `types/calls.rs` exemption, or `range` staying a built-in | M6 step 4 |
| 3 | llm-ergonomist | on these three shapes: **≥30%** of first-try programs off-by-one or avoiding `range` under the silent spec, **<5%** once stated | next harness run |
| 4 | llm-ergonomist | **≥20%** of `sum_to` solutions use a hand-rolled `while` under the silent spec, near zero once stated | next harness run |
| 5 | llm-ergonomist | the sharpest instrument, and it needs no knowledge of the implementation: **the disagreement rate between attempts on the same task** is material under the silent spec (some print 15, some 21) and **zero** once stated | next harness run |
| 6 | spec-warden | deleting `enum Tier` and the `tier:` field leaves `cargo test` **green with zero golden `.expected` touched** — and if it fails, the removal was wrong | this step |
| 7 | spec-warden | **counter-prediction against its own approval**: inclusive-`b` off-by-ones from the unamended spec are **≤5%**, Python's prior dominating — in which case the sentence is funded by being cheap, not by its rewrite rate | next harness run |
| 8 | historian | with names reserved, **zero** collision-class defects; the first library defect is **diagnostic provenance** instead | M6 close |
| 9 | historian | no fully-precompiled library is achievable; the cache boundary lands at parse+typecheck | M8 |
| 10 | ffi-pragmatist | at M8a with the emitter unchanged, the first two-module program where both modules call `range` is `ld: duplicate symbol` at all three levels, surfacing as **exit 2** | M8a |
| 11 | ffi-pragmatist | **§4.19's ladder is untouched**: SQLite step 3 calls no Tier-2 function and needs no shim under any candidate | M7 |
| 12 | ffi-pragmatist | a prebuilt library `.o` will contain **zero** `map` instantiations, so the compiler must emit them into user units — P-c becomes P-source for generics | M6 close |

**Scored here.** Panel 027 #9 — *"spec ≤ 2350 measured at M6 close"* — is
**refuted** by this step at 2354. Recorded as refuted rather than renegotiated:
the step had to repair a self-contradiction nobody had priced, and a signature
costs more than a wording. Panel 026 #6 (< 2450 through 2026-09-30) is alive
with 96 tokens of headroom. Panel 027 #8 is **dormant, and its author says so
first** — no `sort` element-type sentence landed, so a conditional whose
antecedent never fired will be scored "no information" rather than claimed as a
win.

## Watch list

- **The convener's framing was wrong in six places and the engineer listed all
  six.** `heroes outline` does not exist (it is an aspiration in the help text,
  not a verb); the double-emit determinism test is **not** at risk, because it
  compares two emissions of the same input — what breaks is five
  `tests/golden/emit/*.expected` snapshots; the resolver needs no dispatch
  change, because `resolve/exprs.rs` consults `top` before `index_of`; the
  `prelude.hero` name collision is already impossible, because reserved names
  cannot be declared; design.md never uses the word "prelude" at all.
- **`ok` is reserved by the compiler and absent from the spec's built-in list**
  (the warden, while verifying something else): `function ok(x: int)` is
  `error[builtin_name_taken]`, but "None of these names may be redeclared"
  cannot cover a name the list does not contain. Loud, one round trip, and
  therefore not funded — but the new `resolve/tests/spec.rs` invariant does not
  catch it either, because `ok` *is* mentioned in the spec, three lines away, as
  `` `.ok x` ``.
- **`emit/decls.rs` is 311 lines**, already past §11's ceiling before this step
  adds to it.
- **Every array built in Heroes is O(n²), and `range` is the first place a user
  meets it.** Measured at −O2: `sum_to` via `range` takes 0.53 s at n=25 000,
  2.06 s at 50 000, 8.17 s at 100 000 — a clean 4× per doubling, because
  `hero_array_push` allocates an exact-size array and copies every element.
  `range(0, 10000000)` does not terminate; the fitted projection is ~22 hours.
  **This is design.md §4.10's declared bill and Part 2 forbids using it as an
  argument, so it is not one** — it is recorded because it bears on a question
  the M6 closure-list audit owns: whether a library written in Heroes can carry
  the closure list at all. The FFI-relevant half is not the time: it is that
  `hero_array_new` aborts on allocation failure, so **`range` can fail where a
  counting loop cannot**, and an `abort()` inside a `sqlite3_exec` callback or a
  raylib frame kills the process with the library's locks unresolved.
- **`collect_sources` filters `.c` and `.h`**, so a `.hero` file under
  `runtime/` would not enter the cache key — measured: adding and then editing
  one left the build key unchanged. Harmless under P-embed, which puts no file
  there, and a one-word fix the day anything else does.
- **design.md does not cover the linkage question at all.** §4.1 says "no modules
  in v1"; §4.12 says monomorphisation generates "a copy of the function" and
  never says which translation unit the copy lands in. R3b is therefore a **new
  rule**, not a citation, and it is written down as one.
- **Nim's `system` and Haskell's `Prelude` are the warnings, not the models**
  (historian): both grew because growth was free, and Haskell's is now being
  un-shipped for making partial functions the easiest thing to reach. Go's
  `builtin` — closed, non-redeclarable, documented as a fiction — is the closest
  working precedent, and it has held sixteen years.
- **Rust's prelude-edition precedent does not bind, and the convener cited it as
  if it did.** RFC 3114's hazard is *trait* method resolution; free functions
  were always addable. Heroes has no traits and no overloading, and already
  forbids redeclaring these names — so Rust's silent misresolution is a loud
  error here.

## DESIGN-LOG

Appended 2026-08-11 — see the lines citing panel 028.
