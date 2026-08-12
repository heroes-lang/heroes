# Panel 036 — The FFI ladder, and the two silent-wrong-answer routes it nearly shipped

**Convened** 2026-08-12, at the opening of M-ffi-ladder (the first milestone
whose acceptance test is a real C library).
**Trigger** `spec/**`, design.md §4.19 and §1.11, surface syntax, and the tool
surface (CLAUDE.md §4).
**Status** `ratified — 2026-08-12, blanket author instruction` (see § Ratification).

## The proposal, verbatim

> P1. **Header + link attach to an `extern` group.** Head line, then indented
> signatures — the shape `record`/`variant` already have:
> ```
> extern "sqlite3.h" link "sqlite3"
>     function sqlite3_open(path: cstr, out: ptr) -> int
>     function sqlite3_close(db: ptr) -> int
> ```
> The emitter emits `#include "sqlite3.h"` once per distinct header (the quoted
> form, which falls back to the system search path) and passes `-lsqlite3`.
> P2. **A shim is declared, not compiled by hand**: `compile "shim.c"` on the head
> line; `heroes build` compiles and links it. **`heroes cc` never enters the CLI
> surface** (CLAUDE.md §10's stopping rule), amending design.md §4.19.
> P3. **macOS frameworks**: `framework "IOKit"` on the head line → `-framework IOKit`.
> P4. **The closure list's last three rows become built-ins**: `read_file(path: str)
> -> str?`, `write_file(path: str, text: str) -> ()?`, `args() -> [str]`,
> `exit(code: int)`. Panel 030 R3 established that plain `extern`s cannot express them.
> P5. **The ladder**: printf → libm → SQLite (acceptance: open, query, close, no
> shim) → raylib.

Measured before the session: status quo **2422** · P4 **+84** · P1–P2 **+114** ·
package **+198**.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| ffi-pragmatist | **adopt-with-amendment**, 4 conditions | compiled both halves of a fork the proposal did not know it contained: re-declaring gives **5 `conflicting types` out of 5** on SQLite, and include-only makes `extern function sqrt(x: f64) -> int` **compile clean and print `1`** |
| compiler-engineer | **veto P4's `read_file`/`write_file`** · object P2, P3 · adopt the rest | §1.7's mechanical test: a `T?` is a **per-translation-unit generated struct** named by index, so the C runtime *cannot* name one — `emit/builtins.rs`'s one-name-one-symbol contract cannot express `read_file` |
| spec-warden | **object** · veto P2 and P3 · **+119, not +198** | found **a closure-list row nobody had counted**: after the archive, self-hosted `heroes build` must spawn clang, and `int64_t system(const char *)` is `conflicting types for 'system'` |
| llm-ergonomist | **object to both FFI drafts** · prefers the group · declines its veto | wrote the acceptance program and **could not**: nothing in the language produces a `ptr` value, so `sqlite3_open(path, &db)` is unwritable. Task 1 is a non-program in both variants |
| historian (advisory) | approve P1, P2, P5 · amend P3 · **object P4** | **Pascal built in all three of P4's items and Kernighan itemised all three as defects; Wirth removed every one of them from Oberon.** Odin redesigned `core:os` in October *because it was a package* |

## Two silent-wrong-answer routes, both found by compiling

This is the session's result. The proposal contained two ways to ship exactly the
class of defect this language exists to kill, and neither was visible by reading.

**The quoted include takes a decoy, in silence.** With a planted `sqlite3.h`
beside the generated unit in `build/<hash>/`, `#include "sqlite3.h"` compiled with
**zero diagnostics under `-Weverything`** and `clang -E` confirmed it had read the
decoy. This is panel 020's decoy-`runtime/` finding relocated to the header — and
the `_Static_assert(HERO_RUNTIME_ABI == 10)` that rescued it there **cannot be
written for a foreign header**. The form was chosen for the author's own
`mylib.h`, and it fails that case too: the translation unit lives in `build/`, so
`""` searches the wrong directory (`fatal error: 'mylib.h' file not found`).
**Adopted: emit `<header.h>`, and pass `-I<directory of the .hero source>` after
the runtime's.** Both cases pass, measured.

**The §4.19 guarantee was about to become false.** The proposal never said whether
the emitter re-declares the signature. Both prongs were compiled:

| | result |
|---|---|
| re-declare + `#include` | `conflicting types` × 5 of 5 on SQLite — Heroes `int` is `int64_t`, every entry point returns C `int`. Ladder step 3 unreachable |
| `#include` + call only (Nim's `importc`) | SQLite opens, queries, closes, prints `42`, **no shim** — and `extern function sqrt(x: f64) -> int` compiles clean, exit 0, **prints `1`** |

Four wrong bindings out of six compiled **clean**: wrong return type on
`sqlite3_step`, wrong return type on `sqrt`, wrong parameter type on
`sqlite3_column_int`, `ptr` where the header says `const char *`. §4.19's sentence
— *"a wrong FFI type is a compile error, not a runtime disaster"* — was false for
the pair the ladder's own second rung uses.

**The repair is one line per `extern`, compiled, 11 of 11.** A `_Static_assert`
over a `_Generic` whose controlling expression is a call that C11 6.5.1.1p3 does
**not evaluate** but does type-check:

```c
#define HERO_RET_INT(call) _Generic((call), \
    signed char:1, short:1, int:1, long:1, long long:1, default: 0)
_Static_assert(HERO_RET_INT(sqlite3_open((const char*)0,(void*)0)), "extern sqlite3_open -> int");
```

No re-declaration, no `conflicting types`, zero runtime cost. Every correct
binding across the whole ladder passes; the wrong ones fire with a readable
message; and `size_t` — panel 030 R3's third row — turns from silent into a
compile error, because the widening set admits signed C integers and refuses
unsigned. **Adopted, as the mechanism that makes §4.19's sentence true.**

## The disagreements

**P4 lost 3–1, and the one vote for it was the strongest reader argument.** The
compiler-engineer vetoed on §1.7 (`read_file -> str?` needs a second
`print`-shaped composition arm in `emit/ops.rs` **plus an `HERO_RUNTIME_ABI`
bump**, because a `T?` is a per-TU struct the runtime cannot name). The historian
produced the Wirth arc — Pascal predeclared file I/O, arguments and termination;
Kernighan's 1981 paper names all three as defects (*"There is no notion of access
to command-line arguments"*, *"no standard way to terminate execution"*); Oberon's
predefined-identifier list contains **none** of them. The ffi-pragmatist compiled
the alternative and it runs, leak-clean.

But the llm-ergonomist, reading only the spec, reported that **file I/O through
the FFI as specified is impossible, not merely awkward**: `fopen`/`fread` need a
`FILE*` it cannot obtain and a buffer it cannot allocate — no byte-buffer type, no
allocation, no `cstr`→`str`. *"`read_file` returning `str?` is the only reachable
design."*

**Both are right, and the resolution is that they were answering different
questions.** The ergonomist's question is what the *spec* says; the engineer's and
the historian's is where the *code* lives. So the spec names the four functions
and their signatures — and says nothing about the route. They are delivered as
**Tier 2, written in Heroes over `extern`s against a `hero_os.h` this project
ships**, which the ffi-pragmatist compiled, linked and leak-checked before the
session ended. Zero new backend forms, zero built-in rows, and the spec text is
byte-identical under either route.

**Panel 030 R3 is re-recorded, not overturned.** Its three failures reproduce
verbatim on clang 21 — but *only in the re-declaring mode*. In include-only mode
`exit`, `fopen` and `fread` all compile clean. R3's conclusion ("plain `extern`s
cannot express them") is **mode-dependent**, and the record should say so.

**The mortgage's point estimate is withdrawn.** R3 pre-registered ≥60 tokens,
point estimate 84; the first draft measured exactly 84. The spec-warden refused to
score that as a hit: it moved the same four names between **+73 and +84** by prose
alone, and the FFI half between **+63 and +114**. *A quote a rewrite can move by
13% is a prediction about drafting style, not about the language.* **Ledger
convention adopted: a mortgage is registered as a floor, or as an interval with
the drafted text attached and measured.** Score R3 as *floor met · point estimate
withdrawn*.

**A closure-list row was missing, and this was the last milestone that could price
it.** After the archive, `heroes build` and `heroes run` are Heroes programs, and
they spawn clang — `int64_t system(const char *)` is `conflicting types for
'system'`, panel 030 R3's wall on a row nobody had quoted. The audit that produced
the list is *mechanical*, so it structurally cannot find what is absent from it.
Recorded in the ledger; M-selfhost-probe decides it.

## The resolution — provisional, author ratification pending

1. **P1 adopted as the group form, flattened in the parser.** One `Decl` per
   `extern function`, two `Option<Span>` fields on `Function`, and no later pass
   ever learns the word "group" — the compiler-engineer's condition, and without
   it `Decl` index *is* function identity in five modules. Precedent is broad
   (Rust `#[link]` on `extern` blocks, cgo, Odin `foreign`, D `pragma(lib)`), and
   the ergonomist's case is drift: at twelve declarations the repeated tail is
   where the one wrong tail hides. Zig's 0.16 retreat from `@cImport` **does not
   transfer** — its cause was libclang inside the compiler, and there is none here.
2. **`<header.h>` + `-I<source dir>`**, never the quoted form. Held hardest by the
   judge who compiled it.
3. **One `_Generic` `_Static_assert` per `extern`**, so §4.19's sentence is true.
4. **The four names are spec'd, and delivered as Tier 2 over `hero_os.h`.**
   `exit` is the exception: it needs `_Noreturn`, and whether a Heroes wrapper
   preserves it is decided by a compiled experiment in step 2, not here.
5. **P2 and P3 wait** — two vetoes and an objection each, no acceptance test at
   this milestone (`compile` has none until a shim is needed; raylib's dylib links
   with `-L`, not with frameworks). `heroes cc` therefore stays unbuilt **and
   undecided**; design.md §4.19's sentence is not amended yet.
6. **What P1 actually lacks is search paths, not a framework keyword**:
   `-lraylib` fails on a clean machine without `-L/opt/homebrew/lib`. The
   ffi-pragmatist's `lib_dir`/`include_dir` (paths validated not to begin with
   `-`, so the form is closed rather than an arbitrary-flag hole) is the shape,
   and it lands when a rung needs it.
7. **`ptr` gets exactly one literal, `nullptr`, and a C out-parameter is an `@`
   parameter** — CLAUDE.md §7 already makes `@` a pointer parameter. Without this
   the acceptance test is unwritable, which the ergonomist demonstrated by failing
   to write it.

   **Amended the same session, by an instrument rather than a judge.** The rider
   said `null`, and `measure::spec`'s gate — which asserts the spec never uses a
   word the lexer rejects — turned red: `null` is in the foreign-word registry,
   answered with *"there is no null in this language — absence is a fallible type:
   `int?`"*. That rule is right and stays. What was wrong was the **message**,
   which at a `ptr` site sends a binding author to `int?`, and no C out-parameter
   can be one; it now names `nullptr` as a C pointer's zero. Five judges read the
   proposal and none saw the collision, because none of them is a program that
   checks the spec against the lexer.
8. **The ladder gains a rung, by author instruction (2026-08-12): libcurl,
   between SQLite and raylib.** It is the rung that tests what the others do not —
   `curl_easy_setopt` is **variadic** (panel 013's Apple-ARM64 on-stack variadics)
   and `CURLOPT_WRITEFUNCTION` is a callback, which §4.19 fixes at `ptr`.

**Spec cost of the adopted package: 2422 → 2560, net +138**, including the
spec-warden's two removals (−31): the reserved-bitwise clause (line 143) and
*"`?` on a non-fallible value is a compile error"* (line 131). **Both removals
carry conditions and neither lands without its condition**: `6 & 3` currently says
`` `&` is not part of the language's syntax (ASCII-only) ``, which is **false** —
`&` is ASCII — and must become `reserved_operator` naming the six characters; and
`not_fallible` must carry a `certain` fix deleting the `?` span, which it does not
today.

**What a veto would compel.** If the author overturns the P4 resolution toward
built-ins, `emit/ops.rs` gains a second `print`-shaped arm and `HERO_RUNTIME_ABI`
leaves 10 — and the compiler-engineer's prediction below becomes the test. If the
author overturns the group form, the trailing clause is ~26 lines against ~75 and
nothing else in the resolution moves.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| ffi-pragmatist | SQLite open→`prepare_v2`/`step`/`column_int`→close links with `-lsqlite3` and needs **no shim** — but reading a TEXT column into a `str` **will** need one, because `sqlite3_column_text` returns `const unsigned char *` and no `cstr`→`str` route exists | M-ffi-ladder step 3 |
| ffi-pragmatist | `framework "…"` will **not** be exercised by raylib on this machine (the homebrew dylib links without one) | M-ffi-ladder rung 4 |
| compiler-engineer | the Tier-1 route puts `emit/ops.rs` over **420** lines (today 353) and bumps `HERO_RUNTIME_ABI` past 10; the Tier-2 route leaves it under **380**, ABI at **10**, and `emit/builtins.rs` +<10 lines | M-ffi-ladder close |
| compiler-engineer | the group form puts `syntax/decl.rs` over **420** (today 352) and forces a §11 file split; the trailing clause leaves it under 380 | M-ffi-ladder close |
| spec-warden | the adopted text measures **exactly 2541/2472** (its own draft; the adopted one is 2560 with rider 7) | now — scored below |
| spec-warden | of ten programs that read *and* write a file, **≥30% fail first-try on `-> ()?` alone**, because `ok(())` is a parse error, `ok()` is `wrong_arity`, and falling off the end is `missing_return` | M-program-corpus |
| spec-warden | by M-selfhost-port, invoking clang from Heroes costs **≥25 spec tokens or a `PORT-DEBT` entry** | M-selfhost-port |
| llm-ergonomist | ≥30% of first tries carry a neighbour's `link` onto a new header under the per-declaration form, <5% under the group | Part 11 harness |
| llm-ergonomist | ≥15% of twelve-binding first tries make a group-boundary error under the group form — and **100% of those are compile errors**, none reaching a linker | Part 11 harness |
| llm-ergonomist | ≥50% of first tries write `pow(2.0, 10.0)` positionally, ignoring the same-typed-parameter rule | Part 11 harness |
| llm-ergonomist | ≥80% of first tries invent a `ptr` value (`null`, `0`, `ptr()`) — which rider 7 is the answer to | Part 11 harness |
| historian | `exit(code)` typed as returning `()` either trips `-Werror=conditional-uninitialized` or forces an emitter special case with an unproven `hero_unreachable()` | M-ffi-ladder rung 1 |

**Scored now:** the spec-warden's own draft measures 2541/2472 — **correct to the
token**, and it is the only prediction in this project so far that was checkable
in the same session it was made.

## Conditions on the record

- **compiler-engineer** withdraws the P4 veto on *one compiled counterexample*:
  `hero_os.h`'s `read_file` wrapper either rejected by clang, or leaking under
  `--sanitize` + `hero_runtime_check_leaks()`, or unwritable in today's forms.
  Any one, and it is wrong. (The ffi-pragmatist compiled the negative of all three
  before the session closed — recorded, not scored, because it wrote its own test.)
- **compiler-engineer** requires `printer/tests/mod.rs`'s idempotence and
  canonical-stability assertions to stay green with the re-grouping rule written
  down, since `fmt` must now decide where a group begins.
- **ffi-pragmatist**: `exit` bypasses `hero_runtime_check_leaks()` — measured, a
  program with one live block exits 3 rather than 134. Every golden that exits via
  `exit` silently loses its leak check, and it owes either a stated exemption or a
  test.
- **llm-ergonomist** moves to approve when the FFI section states, in this order:
  how a `ptr` is obtained (rider 7), whether `cstr` converts back to `str`, the
  lifetime and NUL semantics of `s.cstr()`, whether `link` is omissible, and a
  truthful account of what clang catches (rider 3).
- **spec-warden**: neither removal lands without its diagnostic repair, and `()?`
  owes exactly one constructible success spelling with the other carrying a
  `certain` fix.
- **historian**: P4 flips to approve on one language that made *runtime* file
  reading a compiler built-in and shipped it ≥5 years without regret. It found
  none — only compile-time embedding (`@embedFile`, `include_str!`, `slurp`),
  which is the line Principle 0 draws anyway.

## The record's own correction

The historian reported, and flagged, that an automated summary had told it Oberon
predefines both `HALT` and `ASSERT`. Reading Wirth's report directly, **only
`ASSERT` is predefined**. The correction is kept here because it is the shape
CLAUDE.md §1 exists for: the summary was fluent and wrong, and only the primary
source said so.
\n
## Ratification — 2026-08-12, by author instruction

**RATIFIED.** The instruction was a blanket one — *"ratifica tutto"* — given
after the session summary, not a clause-by-clause reading of this file. It is
recorded that way on purpose: this project's rule is that a record must not say
more than what happened, and the shape of a yes is part of what happened.

What it settles: the provisional resolution above **stands as the decision**, and
work no longer proceeds on it as a default. Every resolution here was already
load-bearing, so this moves the record's status rather than the compiler's
behaviour.

What it does **not** settle: anything this file keys to a measurement nobody has
taken. Those stay open on their own terms in `docs/debrief/QUEUE.md`, and a
blanket yes cannot make a number arrive.

Open here after ratification, and each one for a reason a verdict cannot reach:

- **the llm-ergonomist's four first-try rates.** They are predictions about what a
  model does, and the Part 11 harness has not run. A yes to the design is not a
  number.
- **the historian's condition on P4** — one language that made *runtime* file
  reading a compiler built-in and shipped it five years without regret. It found
  none; nobody has looked since.
- **`compile "shim.c"` and `framework "…"`**, which were not refused on their
  merits but deferred for want of an acceptance test. They wait on a rung that
  needs a shim, not on the author.
- **`heroes cc`**, which is therefore still *unbuilt and undecided* — two different
  things, and design.md §4.19 now says so where it used to promise the command.

Scored at close, and all four right: the compiler-engineer's file split (479
lines, over its 420), the spec-warden's token count (exact on its own draft) and
its `-> ()?` failure (no spelling worked at all), and the ffi-pragmatist's
`cstr`→`str` need. The historian's `exit` prediction did **not** fire, and the
reason is on the record: `hero_exit` is `_Noreturn`, so clang's flow analysis
never needed the emitter's help.
