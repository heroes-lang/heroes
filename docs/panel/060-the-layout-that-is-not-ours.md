# 060 — The layout that is not ours

**Status**: `provisional — author ratification pending`.
**Convened** 2026-08-15, `/step` opening **M-struct-passing** (ROADMAP order row 1).
**Lane**: full, five judges. Four of the five compiled; three built in copies with
`rm -rf target build` first, as the house rules now say.

**The proposal's central mechanism survived every attack and its stated argument for
it did not.** The counterexample the brief used to disqualify option B *passes* on
both architectures; the real one is worse than the brief claimed. The clause the
brief offered was false in both directions and would have shipped a third of what it
was bought for. And the two questions the brief treated as concessions — a partial
field list, and a `float` field with no `f32` — turn out to be **free consequences of
option A**, because under A the layout is never ours. That last sentence is the
sitting's finding and this file is named after it.

## The proposal, verbatim

> A `record` may be declared **inside an `extern` group**. The layout is then the
> header's, not this compiler's: the emitter writes no typedef and uses the header's
> own type name unmangled, exactly as it already does for an `extern` function's
> linker name. Every field the program names is checked against the header by a
> never-called probe, so a wrong width, a wrong name or a wrong order is `exit 1` on
> the author's `.hero` line. Fields are the boundary scalars, or another record of
> the same group. Such a record is an ordinary Heroes value otherwise — copied,
> compared, constructed by field name, read with `.field`.

Five questions on the ballot: **Q1** the mechanism · **Q2** whether the field list
must be complete · **Q3** `f32` · **Q4** the diagnostic class · **Q5** the empty
record. The brief's conservative default was A · complete · no `f32` · one code ·
refused.

## The verdict table

| judge | Q1 | Q2 | Q3 | verdict | rests on | measured |
|---|---|---|---|---|---|---|
| **ffi-pragmatist** | **A**, `veto` **B** | **partial** | (iii) **only with the `f64` rule** | approve-with-condition | §1.11, §1.12, §4.19 | `Vector2` under B: sent `(1.25, 9.75)`, arrived `(0.0, -1.13e35)` |
| **compiler-engineer** | A, **object** on its probe | **complete** | (iii), own panel next | object | §1.7, Part 5, §1.0 | ~350 lines built, **470/471** tests green |
| **spec-warden** | — | — | condition **met** | object | §1.6, §1.2, §1.12, §12 | clause +137 → **+60**; removal **−22** |
| **llm-ergonomist** | — | complete **+ a spelling for don't-care fields** | **needed** | approve-with-condition | the two documents | **3 of 30** lines defective without `f32` |
| **historian** *(advisory)* | **A + an assert** | partial is ABI-safe; refuse for **our** reasons | shape (ii) has three precedents | approve | Nim, cgo, bindgen, Dart, ctypes, Haskell | golang/go#7560, rust/54341 |

**No seat defends B and no seat defends C.** Two objections and one veto, all of them
about *how*, none about *whether*.

## Option B is dead, and the brief's own counterexample was wrong

The brief disqualified B — a Heroes struct plus `_Static_assert` on `sizeof` and
`offsetof` — with `{int32_t; float}` against `{int32_t; int32_t}`. The
ffi-pragmatist compiled it in two translation units and it **passes correctly on
arm64 and on x86-64**: `a=7 b=3.250000`. Neither is a homogeneous float aggregate,
so both travel in `x0`/`rdi`. A veto argued from that example would have been argued
from nothing.

The example that kills B is `{float; float}` against `{int32_t; int32_t}` — which is
`Vector2`, raylib's most-crossed struct, 79 direct crossings. Identical `sizeof` (8),
identical `offsetof` (0, 4), every assertion B can emit **green**. Sent
`(1.25, 9.75)`; arrived `(0.0, -1.13e35)`. The disassembly says why: the callee reads
`stur s0,[x29,#-0x8]` / `stur s1,[x29,#-0x4]`, and the B caller emits
`ldur x0,[x29,#-0x2c]` before the `bl`. **A different register file.** In the return
direction x86-64 happened to round-trip and arm64 did not, which is panel 052's
standing veto in its exact shape — silently correct on one leg of CI, garbage on the
machine this project runs on.

ARM's own ABI is the rule underneath (aapcs64 §5.10.5.1: an HFA is a homogeneous
aggregate of a floating-point fundamental type with at most four members), and the
historian found the same hazard reported independently by Dart's implementers, who
list four possible destinations for one struct and note that ARM32 Linux uses FP
registers where Android SoftFP uses integer ones.

**B is not a weaker A. It is an ABI break**, and no assertion in B's vocabulary can
see it. It is also, repaired, strictly worse than A: giving Heroes a `float` field
makes repaired-B into A's probe plus a redundant typedef, and it still cannot express
a `union` (`SDL_Event`, 128 bytes) or an array field (`Material`, `ModelAnimation`).

## Option C costs Heroes more than it cost the language that shipped it

C — refuse by-value structs, write a C shim per library — is Haskell's answer.
Haskell 2010 §8.4.2 lists only scalars and pointers as basic foreign types, GHC 9.14's
FFI chapter still mentions no struct-by-value, and **the refusal held for sixteen
years**. That is the strongest evidence any seat produced for C.

It does not transfer. Haskell holds the line with `hsc2hs`, `c2hs` and `inline-c` —
separate tools that generate the shims. **CLAUDE.md §10 forbids a second binary and
forbids a script**, so under C the shim stays hand-written, by the author, forever.
The two languages that tried C and abandoned it are the other half: Dart's FFI
refused by-value and took it in one release cycle (Dart 2.12), and JNI held for
twenty-five years before JEP 454.

## The clause the brief offered would have shipped 62 of the 191 it was bought for

The spec-warden read the candidate wording — *"Fields are this document's types or
another record of the group"* — against the document rather than against the
intention, and it is **false in both directions**.

`ptr` and `cstr` are **not in § Types**; that table ends at `T?`, and both types exist
only in § FFI's prose. Read literally the clause **forbids a pointer field**.
Measured against `raylib.h` 6.0: **130 of the 192** no-`f32` entry points involve a
struct with a pointer field — `Image`, `Font`, `Mesh`, `Shader`, `Wave`, `Sound`,
`Music`, `AudioStream`, `FilePathList`, `GlyphInfo`, `AutomationEventList`. The
headline of 191 collapses to **62** under the clause's own sentence. In the other
direction it admits `str`, `[T]`, `{K: V}` and `T?`, none of which can have a header
layout, so a reader writes `data: str` for `void *` and pays a round trip.

The llm-ergonomist, blind and from the other side, hit the same wall from inside a
program: it wrote `data: ptr` in its `Image` binding, logged it as a guess, and
predicted 10/10 readers do the same without remarking that `ptr` is absent from the
table the clause sends them to.

**And the clause promised a diagnostic the mechanism cannot emit.** *"A wrong order is
a compile error"* — under A the emitter writes no typedef, construction is by field
name and access is by field name, so **order is immaterial and clang has nothing to
check**. Shipping that sentence would have made the compiler buggy by fiat under
CLAUDE.md §12. The rewrite is silent on order, which is true and free.

## The probe: two seats measured two different forms, and only one of them is sound

Both seats that compile attacked the words *"a never-called probe"* and neither
accepted them.

The **compiler-engineer** measured that a probe in the shape of `extern_probe.rs`'s
existing one — a never-called function over the declared types — is **silent** under
the twelve flags in `flags.rs` on a `float`/`int32_t` field swap, in both the
positional and the designated-initialiser forms. What catches it is type identity:
`_Generic(&((T *)0)->field, <the spelling Heroes declared>: 1, default: 0)`.

The **ffi-pragmatist** measured a hole in `_Generic` — and in the *other* form of it.
`_Generic(((VrDeviceInfo*)0)->lensDistortionValues, float*: 1, …)` **passes**, because
C11 6.5.1.1p2 applies array-to-pointer decay to the controlling expression, so a
`float[4]` field binds silently as `ptr`, 8 bytes over 16. Its remedy is a
`sizeof(field) == sizeof(C)` conjunct.

These are not in conflict and the difference is one `&`. The engineer's form takes the
**address** of the field, and `&` does not decay its operand: `&((T*)0)->arr` on a
`float[4]` has type `float (*)[4]`, which is not `void **` and not `float *`, so the
array case is refused by construction rather than by a second check. The address-of
form is strictly stronger.

The spec-warden's condition (4) — *"type identity, never `sizeof`/`offsetof`"* — is
the same ruling from the budget seat, and it composes: **`sizeof` is never the check;
it may be a conjunct.** The resolution keeps the conjunct anyway, on CLAUDE.md §11's
loud-direction rule: it costs nothing, and it closes a measured hole in a neighbouring
form that a later refactor could drift into. The `float[4]` case is owed a golden that
fires.

## Q2 — three judges, three routes, one answer the ballot did not contain

The ballot offered *complete* or *partial*. Every seat that looked at it found the
same two facts and they cut across both options:

- **Partial is safe to read.** The ffi-pragmatist bound `SDL_Event` — a `union`, 128
  bytes — with **1 of ~40 members named**, took its address through `@`, and round-
  tripped `SDL_PollEvent` against real SDL3: `polled event type = 256`. `Font` from
  `GetFontDefault()` was copied and passed by value with 3 of 6 fields named,
  `sizeof(Font) = 48`. This works because **the emitter declares nothing and clang
  computes the size** — the historian confirms it is Nim's position and cgo's, and
  that in Nim *incomplete is the default* while `completeStruct` is the opt-in that
  buys compile-time `sizeof`.
- **Partial SEGVs on construction.** `Font h = {.baseSize=20, .glyphCount=95}` is
  well-defined C that zero-fills `recs` and `glyphs`; real `MeasureTextEx` gives exit
  139, and under `--sanitize`: `AddressSanitizer: SEGV on unknown address 0xc`. That
  is design.md §1.12 violated starting from legal C.

The historian adds the fact that decides how this gets *recorded*: partial-plus-
by-value is **ABI-safe** under A, so the reason to refuse it is Heroes-side and not
C-side. *"A future panel reading 'partial refused' will assume an ABI reason that does
not exist, and will not revisit it."*

The compiler-engineer priced what "Heroes-side" means, and it is more than a flag.
`emit/perfn.rs:60-61` emits `_eq` and `_hash` for **every** aggregate unconditionally
and `descriptors.rs:270` puts `_hash` into the `HeroDesc`. A partial record cannot
answer `==` honestly — `Color(r,g,b)` would equal `Color(r,g,b)` with a different `a`
— and §4.3 makes `==` structural; dropping `hash` puts a null in the descriptor,
which panel 022 measured as `SEGV on unknown address 0x0, pc 0x0`, no type name and no
line. So partial is **a second kind of type**: a second arm in
`perfn::every_generated_type`, a second table in `descriptors::generated`, and a
refusal at every container site.

**The disagreement is real and it is about timing, not about facts.** The
ffi-pragmatist wants partial now, because without it SDL3's real surface is
unreachable and raylib stops at 337 of 349; the engineer wants complete now, because
partial is a type kind and not a flag.

## Q3 — the question reverses, and `f32` stops being about reachability

Panel 052 struck `f32` because a C `float` **parameter** bound `f64` is bit-exact —
true, and it is true because clang converts against the real prototype.

**Inside a struct there is no prototype.** The spec-warden compiled a Heroes
`record Vector2 { x: f64, y: f64 }` over the header's `Vector2` under the exact twelve
flags: **exit 0, zero diagnostics**, and `_Static_assert(sizeof(((Vector2*)0)->x) ==
sizeof(int32_t))` **passes**. That is option B's veto arriving field-wise, and it
means the measurement that funded 052's refusal does not reach fields.

The warden then showed the condition its predecessor attached to that refusal — *"A1
returns the day a program needs a `float` **value** Heroes must hold"* — is met by
measurement rather than by assertion: of the 157 blocked entry points, **116 only
take** a float-struct and 41 return one, so a pass-through escape cannot reach them.
The program must *construct* a `Vector2`, and that is the value.

And the llm-ergonomist, which never saw any of this, reported the reader's half:
`f64` is *offered* and the right type is *withheld*, so there is no error to hit at
the point of the mistake. It wrote `x: f64` knowing it was wrong — *"not because I
thought it was right, but because Document 1 offers nothing else"* — with 3 defective
lines of 30, against 0 of 30 in the variant that has the row. It called the
first-try-correct rate **0, not low**: `Vector2` cannot be written correctly in that
language. Worse, the document *names the hazard* one line above the example and then
makes compliance impossible, *"which converts a reader who is paying attention into
one who writes the bug anyway"*.

**Then the ffi-pragmatist inverted the whole question.** Under A the layout is never
ours — so a field declared `f64` over a header `float` is **ABI-safe**, not a
compromise. Measured against real raylib: `Vector2 { x: f64, y: f64 }` calling
`CheckCollisionPointRec` (correct 1/0) and `GetSplinePointLinear` (`0.5, 1.5`,
exact), `sizeof(Vector2)` still 8, zero warnings. What refuses it is not C — it is
the probe, if the probe demands exact type identity on fields.

So `f32` is **`f32`-as-a-field**, which is the only thing that was unreachable —
`extern_assert.rs:139` already reads `HERO_RET_F64(c) _Generic((c), float:1,
double:1, long double:1, default:0)`, so an `f32` *result* has been reachable by
widening since M-ffi-ladder. With the field rule, `f32` becomes a question of
**precision** and not of **reachability**, and can be deferred honestly instead of
crippling the milestone.

The engineer priced what deferral saves. `Ty::F32` in `types/table.rs` produces
**exactly 2 rustc errors** and leaves **19 silent sites** across 15 files — the same
number `widths.rs`'s own module doc records for the integer widths (2 errors, 18
silent sites), the same trap, and the file already names the answer: the variant must
carry the width. Shape (i) is 400–550 lines over 20+ files in two languages and bumps
**`HERO_RUNTIME_ABI` 13 → 14**, invalidating every cache key and every golden. Shape
(ii), boundary-only, is *narrower, not cheaper*: still a `Ty` variant, still all 21
sites and 19 fallbacks, ~20% saved. The historian found (ii) shipped three times —
Dart's `Float` (*"not constructible in Dart code and serves purely as marker in type
signatures"*), `ctypes.c_float`, and Erlang's bit syntax, which has carried a
32-bit float for decades with no pressure to become a value type.

## The numbers, corrected

The brief's *"601 of 601 with `f32`"* is wrong, and the correction is not about floats.

| | raylib 6.0 | note |
|---|---|---|
| entry points | 601 | |
| cross a struct by value | 349 | |
| **bindable today** | **252** | panel 052 predicted "not past 254" — **held** |
| complete list, no float rule | **443** (191 of 349) | |
| **complete list + the `f64`-over-`float` field rule** | **589** (337 of 349) | the 12 missing are blocked by **array fields**, not floats |
| partial + `f32` | **601** (349 of 349) | |

`sqlite3.h`: **0 of 288** entry points cross a struct by value — SQLite gains and
loses nothing here, which is the fact that corrects the brief's other framing.
SDL3: **15 of 1271** by value, 14 of them needing partial; SDL's real dependence is on
the addressable local, which is 100% partial-dependent.

The spec-warden ruled on the argument as well as the number: *"a third of every C
boundary is unreachable"* is an average doing the work a per-library claim should do.
§1.12 quantifies over **libraries** — *any C library must be bindable* — and raylib
at 58% unreachable satisfies it on its own. **Not elegance in a principle's coat; a
headline number in a stronger claim's coat.** Fix the argument, keep the milestone.

## Principle 0, stated plainly because no seat let it pass

`runtime/hero_os.h` declares no struct. §1.0's compiler-need branch is **empty** and
both compiling seats said so unprompted. This enters on §1.0's second leg and on
§1.12's completeness clause, ratified by the author 2026-08-14 at `DESIGN-LOG.md:299`.
The compiler-engineer's sentence is the one to keep: *"191-of-349 is a reach count,
not a Part 11 metric… that is admissible, and it is the weaker leg — the proposal
does not say so, and should."* It says so here.

On the ceiling: `crates/heroes/src` is **31,514** non-test lines against Pascal-P4's
~4,000, already 7.9×; +600 is +1.9%. The engineer declined to invent a size objection
and said the ceiling is not where this proposal is wrong.

## The provisional resolution

Adopted as the most conservative reading that every measurement supports. Work
proceeds on it; the author's verdict is appended when given.

- **Q1 = A.** B is **vetoed** (ffi-pragmatist, on `Vector2` measured). C is refused —
  Haskell's answer, held for sixteen years, and unaffordable here because §10 forbids
  the tools that made it survivable.
- **The probe is `_Generic` on the *address* of the field**, in the engineer's
  measured form, **with the `sizeof` conjunct** the ffi-pragmatist asked for. Never
  `sizeof`/`offsetof` as the check — both were measured blind to `float` vs `int32_t`.
  A `float[4]` field owes a golden that fires.
- **Q2 = complete**, and the record states that the reason is **Heroes-side and not
  ABI-side**: construction, `==`, `hash`, and `perfn.rs`'s unconditional emission.
  Partial is **queued, not refused** — with the ffi-pragmatist's exact permission list
  (read, copy, pass, return) and its exact refusal list (construction target,
  `==`/`hash` operand, map key), and the engineer's price (a second type kind).
  Complete-first is the reversible order; partial-first is not.
- **Q3 = (iii), not now — *and* the `f64`-over-`float` field rule ships with A.**
  That pairing is the sitting's substantive move: it is what makes (iii) honest rather
  than crippling, taking raylib from 191 to 337 of 349 without a new `Ty` variant, a
  runtime ABI bump or 19 silent fallback sites. `f32` returns as a **precision**
  question with its own sitting.
- **Q4 = three codes**, not one: `ffi_field_type` for a width or type mismatch; the
  existing `ffi_unknown_name` for a field the struct lacks; and a new one for a header
  that spells the type `struct Point` with no typedef — **measured today as exit 2**,
  the compiler blamed for a mistake in a `.hero` file, which is exactly what
  CLAUDE.md §7's named exception exists to prevent.
- **Q5 = refused**, at zero cost: `syntax/data.rs:28` already emits
  `error[empty_record]`, verified to fire inside a group. The ffi-pragmatist's
  `_Static_assert(sizeof(T) > 0, …)` is taken anyway for a *declared* record whose
  header type is incomplete, since it also catches a name the header lacks and feeds
  `ffi_unknown_name`'s existing `guess`.

**The spec lands at the spec-warden's wording, not the proposal's.** Measured,
cl100k binding, `heroes measure`:

| | tokens | delta |
|---|---|---|
| today | 3235 | — |
| the brief's clause | 3372 | +137 |
| the warden's rewrite | 3295 | +60 |
| rewrite + the `spec:207` removal | 3273 | +38 |
| **+ the `f64`-over-`float` field clause** | **3291** | **+56** |

+56 against a project mean of +57.2, for a clause that delivers 337 of 349 where the
brief's +137 delivered 62. **The removal is `spec/heroes-spec.md:207**, and it is a
§12 repair as much as a payment: *"Only `-I`, `-L`, `-l`, `-F` and `-framework` are
accepted back"* is **false today** — `commands/libraries.rs:138` accepts nine
spellings, `-D` and `-U` among them, landed by panel 055 because SDL2 was bindable
through neither clause, with the spec never amended. Read under §12 the spec makes
`package "sdl2"` illegal.

## What a veto would compel

The ffi-pragmatist's veto binds only option B, which nothing adopts. Had it bound A,
the milestone's only remaining route is C — hand-written shim C per library, forever,
with §10 forbidding the generator that every language taking that route has shipped.
No seat proposed it.

## Conditions carried into the implementation steps

Each is owed before the spec sentence lands, and each has a seat's name on it.

1. **`printer/fmt_extern.rs` grows a `Record` arm in the same step as the parser**
   (compiler-engineer, reproduced): `heroes fmt` today hoists the record out of the
   group and emits a **different program that still parses**.
2. **A C field named with a Heroes keyword gets a named refusal** (compiler-engineer).
   `in`, `for`, `match`, `struct`, `union` are unbindable under A because the member
   must be spelled as C spells it, and `mangle::field` runs the other direction.
   0 of 35 raylib structs hit it — *"and that is a premise about the world, which
   CLAUDE.md §11 says expires silently"*.
3. **`typedefs.rs::Names::of()` is split into `of()` / `satellite()`**
   (compiler-engineer). One string does two jobs and they part **inside single format
   strings**: `structural.rs:44` is `bool {name}_eq(const {name} *a, …)`, where the
   first must be ours and the second the header's. Twelve call sites, four wanting the
   C type, eight the generated-function prefix. Unsplit, the emitter writes a global
   unmangled `Color_eq` beside the library that declared `Color` — the collision
   `extern_probe.rs`'s own doc records panel 053 finding for probes.
4. **The nested-record field gets its assertion.** The engineer's prototype emits
   **none** for `Outer.pos: Inner` via a `_ => return None` — CLAUDE.md §11's rule,
   in the prototype written to test it.
5. **`ptr` and `cstr` are named in the field list** (spec-warden, llm-ergonomist), and
   the **wrong-order promise is struck** (spec-warden).
6. **The `−22` removal lands in the same commit** as the clause (spec-warden).
7. **Every code gets a `#~`-annotated golden and the `heroes mutate` corpus runs clean
   under `--sanitize`** (compiler-engineer, §9).
8. **The `Font` construction that gives exit 139 today gives exit 1** when partial
   arrives (ffi-pragmatist) — carried with the queued partial item, not with this
   milestone.

## Free findings, none of them this sitting's question

- **`FLAGS` has twelve entries and CLAUDE.md §7 says eleven** — found independently by
  the spec-warden and the ffi-pragmatist. `-Werror=incompatible-pointer-types-discards-qualifiers`
  landed with panel 058 on 2026-08-15 and did not reach the contract. The
  ffi-pragmatist's note is the one worth keeping: *"the file's module doc says it
  exists to stop exactly this drift; the drift resumed the day the file was created."*
- **`ptr` and `cstr` are absent from § Types** (llm-ergonomist, spec-warden), used in
  the `==` bullet 160 lines before they are defined.
- **Character literals are `i64` and C codepoint APIs are `int`**
  (llm-ergonomist): `GetGlyphIndex(font, 'A')` is a compile error and wants
  `'A'.to_i32().must()`. Loud, so it is a win by this project's standard — but it will
  hit every FFI program and § FFI never mentions it.
- **`_ = f(x)` is documented under `match`** (llm-ergonomist) — the only way to
  discard a result, filed in the control-flow section's bullet about `_` *patterns*.
- **panel 052's `"0 of 45 programs change output for want of f32"` was never scored**
  at M-binding-fidelity close (spec-warden). Scored here: **held, and vacuously** — no
  corpus program binds a C `float` in any position. Third instance of row `2675`'s
  finding: a live instrument with no arm for the question.

## Predictions to score

| judge | prediction | milestone |
|---|---|---|
| ffi-pragmatist | `examples/raylib/main.hero` extended with `record Color`, `record Vector2 {x: f64, y: f64}`, `ClearBackground`, `ColorAlpha`, `CheckCollisionPointRec` needs **zero lines of shim C**, and `run/` plus `heroes mutate` under `--sanitize` report zero findings. Falsified if any of the 191 no-`f32` crossings needs a hand-written `.c` or disagrees with the equivalent C program on one input | M-struct-passing close |
| compiler-engineer | `wc -l` over `emit/ffi*.rs` + `emit/extern*.rs` + `assert_spelling.rs` reads **> 1750** (today 1398) and `emit/` total **> 7600** (today 7159). Wrong if Q4 ships in one code under 100 lines | M-struct-passing close |
| compiler-engineer | a `tests/golden/fmt/` case with a `record` inside a group **fails round-trip** unless `printer/fmt_extern.rs` grows a `Record` arm in the same commit. Already reproduced | the step that lands the parser change |
| spec-warden | if the shipped clause's field list does not name `ptr`, `examples/raylib/main.hero` binds **≤62** of the 192 no-`f32` entry points rather than 191. Instrument: `heroes check` + `tests/golden/check/`, whose `ffi_type` arm fires on a struct at the boundary today. Falsified if ≥100 bind with `ptr` unnamed | M-struct-passing close |
| llm-ergonomist | n=10 fresh models, Document 1 + Task 1: **≥9/10** write `alpha: f64` and `x: f64`, **≤1/10** report the header unbindable. Document 2: **≥9/10** write `f32` in all three positions, 10/10 write `0.5` with no conversion | when metric 2 exists (M-selfhost-fixpoint) |
| llm-ergonomist | n=10, Document 2 + Task 3: **≥6/10** write `texture: ptr` for a by-value 20-byte struct field. *Scores the record clause, not the `f32` row* | when metric 2 exists |
| historian *(advisory)* | a golden execution case binding `Vector2` under **option B** — a `{i32,i32}` record over `{float,float}`, identical `sizeof` and `offsetof`, every `_Static_assert` passing — returns **wrong values** on this arm64 machine. If it passes, support for A over B is materially weakened and Q1 reopens | M-struct-passing close |

The ffi-pragmatist's and the ergonomist's Task-3 predictions **point at each other**:
one says the mechanism needs no shim, the other says ≥6/10 readers will write the one
field spelling that is silently wrong. Under the adopted probe that spelling is
`error[ffi_field_type]` on the author's line — so the pair is really a single
question, *does the probe fire where the reader errs*, and the milestone answers it.

## What each lane gave up

Nothing: the full lane ran, and it earned itself twice. The seat that reads only the
spec produced the finding that the clause's field list is unwritable and that the
document names a hazard it makes impossible to comply with; the seat that reads only
precedent produced the correction that Nim does **A and B**, not A instead of B, and
that every numbered silent layout bug sits in the camp that owns its own layout. A
soundness lane would have had neither, and would have shipped a +137 clause delivering
62.

## Author's verdict

**Ratified as it stands, 2026-08-16** (author instruction *"ratifica e sistema
tutto"*, blanket — the fifth of its kind, and the recap of what had already
landed was verified against the tree before the yes was recorded, per CLAUDE.md
§1). The mechanism, the complete field list and the −22 §12 repair that funded
the clause all stand as written above.

**Two clauses of this sitting are NOT ratified, because the author overturned
them himself the day after it sat**, and a ratification that re-asserted them
would rewrite the record rather than close it:

- **"`f32` stays struck"** is dead. `f32` entered 2026-08-15 by instruction
  (*"e aggiungi il tipo f32 basta storie"*). It landed in **this sitting's own
  measured shape** rather than the instruction's — no `Ty::F32`, but
  `Ty::Float(FloatKind)`, because a bare variant beside `Ty::F64` measured 2
  rustc errors and 19 silent sites across 15 files. On landing the rename
  produced exactly 2 exhaustiveness errors, so nineteen sites had been sitting
  inside a `_`, and two of those were silently wrong at exit 0:
  `crosses_the_boundary` answering *"an `f32` cannot cross"*, and `structural.rs`
  generating an `==` that skips the field. `HERO_RUNTIME_ABI` 13 → 14, as this
  sitting's compiler-engineer priced it.
- **The `f64`-over-`float` field clause** — the sentence this sitting called
  *"the one clause that makes the last two survivable"* — was removed the same
  day under §12 (DESIGN-LOG 2026-08-15). Once `f32` exists the field assertion
  asks type identity, so the compiler began refusing `x: f64` over a C `float`
  and the spec was permitting what the compiler refused. `SPEC_TOKENS`
  3347 → 3329. The clause entered and left inside one day, and the ledger says
  so rather than recording only the net.

What that leaves is the sitting minus a deferral its own author reversed, which
is a smaller and truer thing than the sitting as written. Q3's own diagnosis is
why the reversal was right: **inside a struct there is no prototype**, so the
width that is a convenience at a signature is the layout at a field.
