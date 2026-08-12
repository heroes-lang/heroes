# Panel 038 — constants from headers, and the silent error inside the specification

**Convened** 2026-08-12, on the author's question while reading
`examples/curl/main.hero`: *why must constants that are read from outside be set
by hand — can `extern` not carry them too? It is important, full stop.*
**Trigger** CLAUDE.md §4 — surface syntax, a diagnostic class, and spec tokens.
Full five judges: the soundness lane panel 037 opened is excluded by all three.
**Status** `ratified — 2026-08-12, author decision in /decide` (see § Ratification)

## Why it was convened at all, when the record had already decided it

Panel 018 ratified this spelling and then nobody wrote it:

> `018:146` — *"The extern-global spelling is committed at the FFI milestone."*
> `018:180` (watch list) — *"The extern-global spelling (`extern constant name:
> type` or equivalent) — at the FFI milestone at the latest."*
> `018:169` (prediction 4, ffi-pragmatist, checkable at M7) — *"the SQLite
> binding needs an extern-global line — one line under the candidate,
> unspellable under rejected B."*

M7 is M-ffi-ladder (M-ffi-ladder). It closed 2026-08-12 with the constants
hand-copied, and **prediction 4 is not scored anywhere** — not in panel 036, not
in journal 012, not in the DESIGN-LOG, not in the ROADMAP. It came true and was
paid in literals. This sitting exists because a commitment with a deadline passed
its deadline silently, which is the same failure mode the design keeps finding in
programs.

Two other things were already on the record and neither was noticed:

**§4.19 promises this capability in a sentence that is false.** design.md:1786-1789
— *"Macros, `inline` functions and `#define` constants are reachable because the C
compiler sees the real header."* True of the C compiler; false of the Heroes
author, who has no form to name one. This is the exact shape of panel 036 rider 3
(*"without it §4.19's own sentence is false, measured"*), one milestone later, in
the same section.

**The compiler's own library pays for it.** `crates/heroes/src/library/source.hero`
binds `hero_os.h` and compares a status against `0`/`1`/`2`, because it cannot name
`HERO_OS_OK`; `runtime/hero_os.h:36-41` says the codes are *"small integers rather
than errno"* — chosen small so the Heroes side could copy them and stay readable.

## The proposal, verbatim

A `constant NAME: type` item may appear inside an `extern` group. It has **no
body**: the value is the header's.

```
extern "curl/curl.h" link "curl"
    constant CURLOPT_URL: int
    constant CURLE_OK: int
    function curl_easy_setopt(handle: ptr, option: int, value: cstr) -> int
```

One rule comes out of it: **inside a group, a declaration is a signature and not
a definition** — `function` gives up its body and so does `constant`. Outside a
group both require one. No new exception to §4.2's entity form.

Spec diff, one code line and one sentence in `## FFI`:

```diff
 extern "sqlite3.h" link "sqlite3"
+    constant SQLITE_OK: int
     function sqlite3_open(path: cstr, out: ptr) -> int
```
> A `constant` in a group has no body: the header holds its value, so you cannot
> copy it wrong.

Two sub-questions were **already decided by the author** before the sitting, and
the judges were told so: the value is never written in Heroes (header only), and
all seven FFI types are in scope, with `str` and `()` expected to be named
refusals.

## The baseline, measured before the sitting

`docs/measurements/005-magic-constants.md`, and it is the reason this is not a
worry but a number. `heroes mutate` had eleven operators and none touched the
digits of a number, so metric 3 could not see this class at all. The twelfth,
`typo-digit`, moves the last digit of a `constant`'s value by one:

| operator | mutants | excluded | killed (check) | killed (`--permissive`) |
|---|---|---|---|---|
| typo-digit | 5 | 0 | **0 (0%)** | 0 (0%) |

Five sites in `examples/`, and **all five are values a C header owns** —
`CURLOPT_URL` 10002, `CURLE_OK` 0, `CURLE_UNSUPPORTED_PROTOCOL` 1, `SQLITE_OK` 0,
`SQLITE_ROW` 100. There is no sixth site. The mutants are legal programs: `heroes
check`, clang with `-Wall`, the linker, the leak gate and ASan all agree, and
libcurl is asked for an option nobody meant.

Spec cost, measured (`heroes measure`, both tokenisers): **2560 → 2594, +34** on
cl100k_base (the binding number); 2492 → 2526 on claude-legacy; spread 68;
headroom 1536 → 1502. The warden reproduced both numbers exactly and then beat the
draft — see § The wording, measured.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| **ffi-pragmatist** | **approve-with-amendment** (`object` without A1) | Compiled 37 cells against real `sqlite3.h` 3.51.0, `curl/curl.h` 8.7.1, `raylib.h` 6.0. The existing `HERO_RET_*` macros transfer to a constant token **17 of 18** unchanged. `__builtin_constant_p` holds, string literals included — and mechanism B gives clang's words, so it is refused. The generated C gets **four lines shorter per constant**. |
| **compiler-engineer** | **approve-with-amendment** | Built the whole thing: **+221 lines, 15 files, 0 new IR ops**, 511 tests green, `fmt` fixpoint holds, double-emit byte-identical. Sugar, confirmed from the code: `body: Option<Block>` **already** means "the code is C's". And the AST answer is not the obvious one — see below. |
| **spec-warden** | **object** | Reproduced +34 exactly, then beat the draft by 9 tokens. Pre-registered a prediction and had it **scored inside the sitting**: the digit class is 267 sites and this reaches 5. Also found that the two removals this panel meant to spend were **already spent**. |
| **llm-ergonomist** | **approve-with-amendment** | Blind A/B on two label-stripped variants. Task 2 (*set `CURLOPT_URL`*) **is not completable** from today's document without inventing a number: first-try **0.75 vs 0.15**, silent-error rate **0.60**. Guessed which variant was the candidate only after finishing. Then found a defect in the half of the document nobody was asking about. |
| **historian** *(advisory)* | **approve-with-amendment** | §4.19's own provenance, Nim's `importc`, **has a constant form** and Heroes took the function half. Nim's stdlib ships both arms of this experiment in one repo. The mistake class is dated and other people's: rust-lang/libc#1142, glibc 2.42. |

Nobody vetoed. The two seats that compile both approved with conditions; the seat
that measures the document approved with conditions; the seat that measures the
budget objected and its objection stands.

## The digit class is 267 sites wide, and this proposal reaches five of them

The spec-warden's prediction was **pre-registered and then measured in the
sitting**, which is the only prediction in this project's record to be scored
inside the panel that made it. It said: an operator that edits *any* int literal
finds ≥20 sites, kills 0 in both arms, and **≤5 of them are C-header constants** —
so most of the class is unreachable by this proposal.

Measured, with the operator temporarily widened to every `ExprKind::Int` in
`examples/` and then reverted:

| operator scope | sites | killed | of which a header owns |
|---|---|---|---|
| every int literal | **267** | 0 (0%) | 5 |
| a `constant`'s value (shipped) | 5 | 0 (0%) | 5 |

**The prediction is correct, and stronger than it guessed: 5 of 267, 1.9%.** It is
on the record because it calibrates the claim — 038 does not close "wrong numbers
in programs", it closes the one subset where an authority exists to be consulted.
The other 262 are unreachable by anything: `print(9)` mutated to `print(8)` is a
different program, not a wrong one, and no rule can know which was meant. That is
also why the shipped operator is the narrow one: a row that can never move, for a
class where nothing could ever move it, measures arithmetic rather than the design.

**A third finding, from the same scaffold.** Run over
`crates/heroes/src/library/`, the wide operator reported **7 sites, 7 killed
(100%)** — and the number is garbage. `library/source.hero` does not type-check as
a program (`error[builtin_name_taken]: 'range' is a built-in`, because the library
*is* the built-ins), so every mutant was killed by a diagnostic that was already
there. `heroes mutate` does not verify that the base program is clean, so a corpus
that does not compile reports a perfect defence. Queued as an instrument defect;
it also refutes the warden's own suggestion to measure over the library.

## The wording, measured

Every row is the full spec, measured with `heroes measure`; baseline 2560.

| draft | max | Δ | carries |
|---|---|---|---|
| the sitting's draft | 2594 | +34 | the constant line, a no-body sentence with its rationale |
| warden **v-A** | 2585 | +25 | the same claim without the rationale, + the `@out` repair |
| warden v-A + a second verification sentence | 2592 | +32 | as above, + "clang checks it too" |
| **v-B — adopted** | **2587** | **+27** | v-A's wording, the `@out` repair, **and** the ergonomist's amendment 1 folded into the promise that was already there |
| the `@out` repair alone | 2561 | +1 | nothing to do with this proposal (see below) |

v-B is +27 for strictly more than the draft's +34: it says the value is the
header's, it says clang checks it, and it repairs a defect the sitting found. The
warden's own criterion picks it, and the rationale it drops is hosted free in
design.md §4.19.

The adopted `## FFI` text:

> A group names its header and its library, and clang checks every signature
> **and constant** against that header, so a wrong FFI type is a compile error:
> ```
> extern "sqlite3.h" link "sqlite3"
>     constant SQLITE_OK: int
>     function sqlite3_open(path: cstr, @out: ptr) -> int
>     function sqlite3_close(db: ptr) -> int
> ```
> A group's `constant` has no body: the header holds the value.

The ergonomist's **amendment 2** — a sentence saying a bodyless `constant` exists
only in a group and a group's `constant` never has a body — is **not** taken as
spec text. Both crossed cases already have diagnostics (`missing_body` for the
first; the second is `extern_has_body`'s sibling), and panel 035's mechanism says
a sentence the compiler says better is not worth its tokens. Goldens for both
directions are the condition of that refusal, not an optional extra.

## The provenance took half the mechanism

design.md §4.19:1856-1860 names Nim's `importc` as this design's provenance. The
historian's sourced finding is that **`importc` has a constant form and Heroes
took only the function half**:

> Nim manual — *"When `importc` is applied to a `let` statement it can omit its
> value which will then be expected to come from C. This can be used to import a C
> `const`."* (`let` form merged 2020-05-12, PR #14258.) The manual's own `nodecl`
> example is a `#define`: `var EACCES {.importc, nodecl.}: cint`.

Nim's standard library ships **both arms of this panel's experiment in one repo**:
`lib/posix/posix_other_consts.nim` imports (`var E2BIG* {.importc: "E2BIG", header:
"<errno.h>".}: cint`), and `lib/posix/posix_linux_amd64_consts.nim` hand-copies
(`const E2BIG* = cint(7)`, `const EACCES* = cint(13)`) — the second exists because
the first cannot be a compile-time constant.

And the mistake class is dated, sourced, and other people's:

- **rust-lang/libc#1142** (2018-11-22), from the maintainers: *"We have many bugs
  where the value of a constant does not match the value of the constant in the
  users system."*
- **glibc 2.42** (2025-07-28) redefined `speed_t` so the `Bnnn` baud constants
  became the baud rate itself; rust-lang/libc#4697 is the repair.
- **LKML, 2025-01-08**: `CPUFREQ_NEED_UPDATE_LIMITS` is `BIT(0)`, and bindgen
  produces nothing for it.

**Nobody verifies a hand-written constant.** The only instance the historian found
is `rust-lang/libc`'s `ctest`, which is out-of-band CI tooling, not a language
form. That absence cuts *for* the proposal: hand-copying is what everyone does
when the mechanism is missing, and it is what breaks.

### The historian's amendment, and why the code already answers it

> *"Nim, Swift and Rust independently refused to make an imported C macro a
> compile-time constant: `let`, `var { get }`, `static`. Nim closed the `const`
> RFC as not planned in 2019. Spelling this `constant` promises what three
> languages declined to deliver."*

The objection is right about the three languages and wrong about what the word
promises **here**, and the difference is in the tree rather than in an argument.
A Heroes `constant` has never been a compile-time constant:

- it is lowered to a **zero-argument function**, and reading its name is a *call*
  (`ir/exprs.rs:174-181`, `ir/mod.rs:93-96`);
- the checker never needs its value — a name's type comes from the declaration
  (`types/exprs.rs:210`);
- **there is no constant folding anywhere** in `types/` or `ir/` (grepped);
- it cannot appear in a `match` pattern: `syntax/control.rs:170-213` accepts
  `.case`, `_`, and a literal, and a name is `expected_pattern`;
- there are no array sizes, no `sizeof`, and no VM.

So Heroes' `constant` **is** Nim's `let` already, and has been since M2. The word
means *bound once, forever* (§4.4's rule for `=`), not *folded*. What the three
languages declined to deliver is a thing this language never had.

That leaves the historian's condition — *do not spell it `constant` unless a
compile-time check exists* — satisfied in the stronger direction: a check is being
added, so Heroes refuses the run-time-object case as a **language rule**. Nobody
else does. Zig refuses it by accident, as a comptime-evaluation failure its own
tracker calls a bug (translate-c #134, #207; ziglang/zig #274, #17862), and Nim,
Swift and Go accept it. The historian's own words: *"Heroes would be first, which
is the position worth taking deliberately."*

Its cost is one arm of the `match` this panel's refusal writes, and its benefit is
that `constant stdout: ptr` cannot compile — which is the same sentence §4.2:673
has always carried (*"there is no `variable` entity; mutable globals are
forbidden"*), reaching the one place that could have smuggled one in.

## The silent error inside the specification

The llm-ergonomist, reading only the document, would not stop pointing at a line
that has nothing to do with constants:

> `spec:180` — `function sqlite3_open(path: cstr, out: ptr) -> int`
> `spec:184` — *"A C out-parameter is an `@` parameter."*

The example contradicts the rule four lines below it, on the most-copied line in
the document. It called this its pre-veto and predicted a silent failure. Its
diagnosis of the failure was wrong in detail, and the real one is worse —
**measured, not argued**: that example, copied verbatim into a program, compiles
clean, links, runs, and **exits 0** printing `rc: 21`. Twenty-one is
`SQLITE_MISUSE`: the address of the handle was never passed, so no database is
opened, nothing is written, and the program reports success to the shell. The
compiling example in the repo (`examples/sqlite/main.hero:28`) has always said
`@out: ptr`.

So the document's one FFI example teaches a program that silently does nothing,
and this sitting found it only because a judge was given the document and nothing
else. It is repaired here, in the section being amended anyway, at **+1 token**.

## The AST answer is the opposite of the obvious one, and only a compiler found it

The obvious shape is a sixth `DeclKind::ExternConstant`: a new variant makes every
exhaustive `match` break at compile time, which is this project's whole method.
The compiler-engineer measured it and it is **wrong**, at one site:

```rust
// crates/heroes/src/ir/exprs.rs:190-192
fn is_constant(ast: &Ast, decl: u32) -> bool {
    matches!(ast.decls[decl as usize].kind, crate::syntax::DeclKind::Constant { .. })
}
```

That `matches!` is **not** a `match`, so it is not exhaustive and nothing would
break. A sixth variant makes it answer `false`, and reading `CURLOPT_URL` lowers
to `Op::FuncRef` (`ir/exprs.rs:182`) — a function pointer where an `int` is
wanted, silently. The variant that is *more* compile-checked in aggregate is
*worse at the single site that decides correctness*.

So `DeclKind::Constant` is **widened**, not joined:
`Constant { ty, body: Option<Block>, header, link }`. Of its 12 sites, 6 break at
compile time and 6 keep compiling — five of those correctly, and the sixth
(`printer/scopes.rs:105`, which prints `constant` where it prints `extern sqrt`
for a function) is the one measured silent acceptance and a condition of adoption.

Reusing `DeclKind::Function` with an empty parameter list is refuted outright:
the name would have to be written `CURLOPT_URL()`, which is not the proposal.

## Two judges met at the same fact from opposite ends

The ffi-pragmatist compiled C; the compiler-engineer wrote Rust. Both arrived at
the same line, which is the strongest form of evidence this panel produces:

**the accessor must be mangled.** CLAUDE.md §7 says `extern` FFI names pass
through unmangled by design — and here unmangled is:

```c
int64_t SQLITE_OK(void) { return SQLITE_OK; }     // error: expected identifier or '('
int64_t CURLOPT_URL(void) { return CURLOPT_URL; } // error: redefinition of 'CURLOPT_URL'
```

The macro eats its own definition. `h_main_SQLITE_OK` compiles clean. This is
§7's first named exception to its own mangling rule, and the reason is exact: an
extern **function** is a name the linker must match, so it must survive
unmangled; an extern **constant** is a name the *preprocessor* must expand, so it
must appear only inside the body. Same rule, opposite consequence, because the
two names are consumed by different tools.

## What C will and will not give, measured rather than assumed

The author's instruction was *all seven FFI types*. Compiled, they are not seven
equal rows, and the honest delivery names each refusal:

| type | from a header | how it is decided |
|---|---|---|
| `int` | yes | `#define`s and enumerators both: `SQLITE_OK`, `CURLOPT_URL`, `KEY_SPACE`, `EACCES` |
| `f64` | yes | `M_PI`, `HUGE_VAL`, `DBL_MAX` |
| `cstr` | yes | a string macro is a `const char *`: `SQLITE_VERSION`, `RAYLIB_VERSION` |
| `ptr` | yes | `NULL`, `SQLITE_TRANSIENT`; **`stdout` is refused** — an object, not a constant |
| `bool` | **the assertion decides** | under `-std=c11`, `stdbool.h`'s `true` is `#define true 1`, so it is an `int` and `HERO_RET_BOOL` fires. Measured |
| `str` | **refused in the checker** | a `HeroStr` carries the runtime's magic word; no foreign header has one. Declare it `cstr` and convert with `to_str` (§4.20) |
| `()` | **refused in the checker** | a constant names a value, and `()` is a type, not a value (DESIGN-LOG 2026-08-12) |

The ffi-pragmatist asked for `bool` to be a third checker refusal, and it is
**not taken** — for the project's own reason. Refusing `bool` in the checker
rests on *"no C header defines a `_Bool` constant"*, which is a premise about the
world and expires silently (CLAUDE.md §11); letting the assertion decide rests on
the type of the token in hand, which cannot expire. `constant true: bool` is then
refused loudly, per constant, with the message naming what the header really says.
The two checker refusals are facts about **Heroes** — its runtime builds every
`str`, and `()` has no value — and those do not expire.

Bonus finding, compiled: with the constancy assertion present, raylib's
`RAYWHITE` (a `#define` expanding to a `Color` struct) is refused in **all seven**
types with our message, including the `ptr` cell that was exit 2 without it. The
constancy check is not only a constancy check; it is what makes a struct-valued
header macro an exit-1 diagnostic instead of an internal error.

## The mechanism, as compiled

Per extern constant, three lines of generated C:

```c
_Static_assert(HERO_RET_INT(CURLOPT_URL), "heroes-ffi-return CURLOPT_URL int");
_Static_assert(__builtin_constant_p(CURLOPT_URL), "heroes-ffi-const CURLOPT_URL");
int64_t h_curl_CURLOPT_URL(void) { return CURLOPT_URL; }
```

- the type assertion is the **existing** macro set, unchanged (`emit/decls.rs:202-211`);
- the constancy assertion keeps the failure inside a `_Static_assert` carrying
  *our* message, which is what `emit/ffi.rs:13-16`'s narrowness requires. The
  alternative — a `static const T probe = X;` initializer — fails with
  `error: initializer element is not a compile-time constant`, clang's words, and
  buying it would mean widening CLAUDE.md §7's named exception from a *message*
  to a *generated line*. Refused, measured, and cheaper.
- compile cost for 110 raylib enumerators: **+0.001 s** at `-O0`. Runtime cost:
  zero (C11 6.5.1.1p3 for the type assertion; both are compile-time).

**The one premise that must carry a test.** `__builtin_constant_p` is
optimisation-dependent for exactly one row: a `static` non-`const` object in a
header is 0 at `-O0` and **1 at `-O2`** — and `heroes build` uses `-O0` while
`heroes run` uses `-O2`, so the two subcommands could disagree about whether a
program compiles. No real header among sqlite3, curl, raylib and SDL2 contains
one (0 matches), and a `static` in an included header is per-translation-unit and
unwritable from Heroes. That is a premise about the world, so it is owed a test
that fires when it dies (CLAUDE.md §11): the table asserted at **both**
optimisation levels.

## A1 — a defect in shipped code, found by compiling, fixed in this milestone

The ffi-pragmatist made its verdict conditional on this, and the compiler-engineer
hit the identical output from the other side. Run against the real compiler,
before this sitting:

```
$ heroes build misspelled.hero          # extern function sqlite3_openn
error[ffi_return_type]: `sqlite3_openn` does not return `int");` — that is what
`sqlite3.h` says, and clang read it
  note: … correct the result type, or name the header that declares this `sqlite3_openn`
```

Two defects in one line. `int");` is text from **clang's echo of the source
line**, which repeats the assertion's message verbatim and so matched the marker
— so CLAUDE.md §7's *"it reads only the assertion messages this emitter itself
writes"* was **already false**. And the diagnosis is wrong: the name does not
exist, and correcting the result type cannot fix that.

It was invisible because the good path is right by **ordering** — clang prints
the message before the echo, and the duplicate is dropped by span. A premise
about clang's output order, doing the work of a fact about the line in hand
(CLAUDE.md §11).

Fixed here, and the repair is a fact rather than an order: a line is one of ours
only if it also says `static assertion failed`; and `call to undeclared function`
/ `use of undeclared identifier`, for a name an `extern` group declares, becomes
**`ffi_unknown_name`** — carrying clang's own typo correction as a `guess` fix,
never `certain`, because it is a search over the header's names and not a fact
about this program (CLAUDE.md §8):

```
error[ffi_unknown_name]: `sqlite3.h` declares no `sqlite3_openn` — clang read the header and could not find it
  note: an `extern` names what the header already has (§4.19): check the spelling, or name the header that does declare `sqlite3_openn`
  fix (guess): the header declares `sqlite3_open`
```

This matters more for constants than for functions, and the ffi-pragmatist said
why: once names outnumber signatures 100 to 1, **a misremembered name is the
dominant mistake**, and `ffi_signature` checks types, never names.

## The resolution — the decision is the author's, the mechanism is the judges'

**Status of the decision.** The author instructed this milestone before the panel
sat (*"è importante e basta"*), and settled its two sub-questions in advance: the
value is never written in Heroes, and all seven FFI types are in scope. On those
three points this file is a **retro-record** — it records the real objections and
does not stage dissent. Everything below them is the panel's.

1. **Adopted: `constant NAME: type` as a group member, bodyless.** One rule —
   inside a group a declaration is a signature, not a definition. `DeclKind::Constant`
   is **widened**, never joined by a sixth variant (`ir/exprs.rs:190`, measured), and
   no new IR op exists, so the compiler-engineer's veto condition is not approached.
2. **Adopted: the mechanism above** — the existing `HERO_RET_*` assertion, a
   `__builtin_constant_p` constancy assertion, a **mangled** accessor, and a
   prototype. §7 gains its first named exception to unmangled `extern` names, with
   the reason recorded: linker versus preprocessor.
3. **Adopted: four diagnostics.** `ffi_type` (existing class, extended: a type no
   `extern constant` may have), `ffi_constant_type` (the header refutes the declared
   type), `ffi_not_constant` (the header makes it an object), `ffi_unknown_name`
   (A1, and it covers functions too).
4. **Adopted: the spec at v-B, +27**, which is 7 tokens *below* the sitting's own
   draft and carries strictly more, including the ergonomist's amendment 1 folded
   into the promise that was already there. Its amendment 2 is **refused as spec
   text** and owed as goldens instead (panel 035's mechanism).
5. **Adopted as a rider: the `@out: ptr` repair**, +1 token. It has nothing to do
   with constants and everything to do with why this panel had a judge who reads
   only the document.
6. **Adopted: design.md:1786-1789 is repaired in the same commit**, at zero spec
   cost, whatever else happens. The warden is right that a false sentence is owed
   its repair independently of what funds it.
7. **The spec-warden's objection stands, un-withdrawn.** Its own withdrawal
   condition was *"if header constants turn out to be the majority of killed-nothing
   digit sites"* — measured at 5 of 267, so they are not. What carries the decision
   is the author's instruction, and this record says so rather than manufacturing a
   consensus. Its three strongest points are adopted rather than answered: the
   wording (4), the repair (6), and the order of operations — the instrument landed
   **before** the cure, as `docs/measurements/005-magic-constants.md`, in
   measurement 004's own order.
8. **Refuted, and struck from the case for this milestone: the §1.0 compiler-need
   claim.** `hero_os.h`'s status codes are this project's own, so `library/source.hero`
   could name them with an ordinary `constant` at zero spec cost (panel 034 R4).
   The proposal rests on §1.0's *second* clause only — a measured Part 11 effect —
   and it has one: 5 sites, 0 killed, and a shipped example that asks for the wrong
   libcurl option when one digit moves.

**What a veto would compel.** The compiler-engineer's veto was reserved for a new
`Op` or a `DeclKind::ExternGroup`; neither is used, and if either becomes necessary
during implementation the milestone stops and the panel reconvenes, because panel
036's flattening condition would have to be re-argued. The ffi-pragmatist's
`object` stood on A1 alone, which is fixed and tested.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| spec-warden | the wide digit operator finds ≥20 sites, kills 0, and ≤5 are C constants | **now — scored: CORRECT.** 267 sites, 5 constants, 0 killed |
| spec-warden | the bodied form lands at ≤+15 measured and closes the same five sites | first half scored (**+12 measured**); the second is untested, because the author refused the form |
| llm-ergonomist | under today's spec ≥50% of compiling attempts at task 2 carry a wrong `CURLOPT_URL` and 0% are diagnosed; under the candidate 0% carry any literal | the Part 11 harness at M-program-corpus |
| llm-ergonomist | task-2 first-try X − Y ≥ +0.40; under +0.15 the diff is cosmetic and it downgrades to a shrug | the Part 11 harness at M-program-corpus |
| llm-ergonomist | under today's spec, ≥50% of readers answer that a group `constant` is already legal | the Part 11 harness at M-program-corpus |
| compiler-engineer | `heroes fmt` moves `examples/curl/main.hero`'s four-line comment off the migrated constant unless `printer/fmt.rs`'s first-member path is fixed first | **scored: the condition was met.** Reproduced with functions alone, fixed in step 3, `a_comment_above_a_groups_first_member_stays_on_it` pins it, and the migration moved nothing |
| compiler-engineer | the milestone lands >221 Rust lines and `emit/decls.rs` finishes ≥750 with no split | **scored: CORRECT on both halves.** Net **707** Rust lines; `emit/decls.rs` at **768**, from 717, unsplit. §11's ~300 is breached by 2.5×, the breach predates this milestone and this milestone made it worse — queued with the number rather than argued away |
| ffi-pragmatist | a raylib binding naming all 110 `KEY_*` enumerators plus three flags compiles, links and runs with no shim and no number in any `.hero`, so rung 5's residual shim need is struct passing alone | §4.19's ladder rung 5 |
| ffi-pragmatist | `constant RAYWHITE: T` fails for all seven `T` with an exit-1 Heroes diagnostic, never exit 2 | not tested here: no raylib golden exists, and adding one puts a third-party library on the harness's link line. Rung 5 |
| historian | naming `errno` or `stdout` compiles and runs at exit 0 with zero diagnostics **unless** a constancy check exists — Heroes' mechanism is otherwise more permissive than Zig's, failing in the quiet direction | **scored: CORRECT, and it is why the check exists.** `constant stdout: ptr` is `ffi_not_constant` at exit 1 on the `.hero` line (`fixedbugs/ffi-not-constant.hero`, `a_c_object_is_not_a_constant`). Without the second assertion it compiles and runs |

## Conditions on the record

- **ffi-pragmatist**: A1 fixed (**done, tested**); the accessor mangled; the
  `__builtin_constant_p` table asserted at `-O0` **and** `-O2`, so the
  optimisation-dependent row fires when it changes.
- **compiler-engineer**: widen `DeclKind::Constant`, never a sixth variant;
  `printer/scopes.rs:105` must print `extern constant`; a golden under
  `tests/golden/emit/` or `run/` must contain an `extern constant`, because
  `golden.rs:519`'s double-emit test is otherwise **vacuous for this feature**;
  `emit/ffi.rs` must stop saying *"does not return"* for a constant and
  `externs::body_check` must stop saying *"it names a C function"*; and the `fmt`
  first-member comment path is fixed **before** the examples migrate, or the
  migration is measured and shown not to move a comment.
- **spec-warden**: the after-evidence is a **site count**, never a kill rate;
  `QUEUE.md:339` is closed as already discharged by `6a58d47`; the spec delta moves
  in the same commit with the number in the body.
- **llm-ergonomist**: it flips to `object` if a misspelled or wrongly-typed group
  constant reaches a binary undiagnosed — `ffi_unknown_name` and
  `ffi_constant_type` are the answer, and each owes a golden that makes it fire.
- **historian**: `constant` may keep its name only because a compile-time check
  exists; without one, three shipped languages say the word over-promises.

## What this session cost

Five judges, four of them compiling: two full Rust prototypes (one in its own
worktree, one in C), 37 compiled C cells against three real libraries, eight
measured spec variants, and one blind A/B. Wall clock about forty minutes, run in
parallel.

It bought four things the proposal did not contain: the AST answer is the
opposite of the obvious one; the accessor must be mangled, found twice from
opposite directions; a live defect in shipped diagnostics; and a **silent error in
the specification itself**, on the line every FFI program starts from. Three of
the four were found by compiling rather than by reading, which is now the third
sitting in a row where that was true.

## Ratification — 2026-08-12, by author decision in `/decide`

**RATIFIED in block**, with the spec-warden's objection left standing on the
record rather than dissolved by the yes.

The components put to the author, each ratified: a bodyless `constant` inside an
`extern` group · the value never written in Heroes · `DeclKind::Constant` widened
rather than a sixth variant · the **mangled accessor** as CLAUDE.md §7's first
named exception to unmangled `extern` names · `__builtin_constant_p` as the
constancy check · the four diagnostics (`ffi_type` extended, `ffi_constant_type`,
`ffi_not_constant`, `ffi_unknown_name`) · the spec at **+28** · the `@out: ptr`
repair.

**What the yes does not do is retire the objection.** The warden sized the digit
class inside the sitting — 267 int-literal sites in the corpus, of which this
milestone reaches five — and the objection was that a capability answering 2% of
a class it is argued from is not carried by its own verdict. It is not carried by
this ratification either. What carries it is the author's instruction, and the
record says so in both places (the resolution above, and here) because the
alternative is a file that reads as though a judge had been persuaded.

The scored prediction stays as it was written: this is the first sitting where a
judge's prediction was pre-registered and scored inside the same session, and it
did not go the proposal's way.
