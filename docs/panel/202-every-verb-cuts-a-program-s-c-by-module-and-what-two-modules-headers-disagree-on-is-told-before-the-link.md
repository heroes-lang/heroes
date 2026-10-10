# Panel 202: every verb cuts a program's C by module, and what two modules' headers disagree on is told before the link

Convened 2026-10-09 by the coordinator on the author's yes of about 22:47
(the question widget: *both sittings now, 6 USD between them*), under the
author's instruction of about 19:35, *every defect closed*, for defects 550
and 453 and the shapes beside defect 538. **A full panel** (what `check`,
`build`, `run`, `test` and `--emit-c` say of a program is a diagnostic class):
the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian, the
blind seat in eight fresh `claude -p` sessions outside the repository (2.0552
USD, after a first run of 0.3734 USD the account's session limit stopped with
no report), and the completeness critic before the seats and after them. The
tree frozen at **`46b80b82`**, worktree `lane-panel-202`. Briefs written from
22:53; the critic's first pass read by 23:06, its repairs applied before any
seat started (the question widened from *groups that bind one C name* to
*headers two groups name that cannot share one unit*; four unfiled failures,
two of them a wrong value at exit 0); seats from 23:06; the spec-warden's
reply copied at 23:21, the historian's at 23:21, the ffi-pragmatist's at
23:29; the session limit from about 23:31 to 23:46 stopped the
compiler-engineer and the blind seat, both resumed on the new account; the
blind seat's eight sessions from 23:46:42 to 23:48:04; the compiler-engineer's
reply copied at 23:59; the critic's second pass to 00:10 on 2026-10-10; this
synthesis from 00:16, every time read from `date`. Briefs in `202-briefs/`,
reports in `202-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **object** to (a), (b), (d), (f), (h); withdraws its own (i); **approve (c) with (g)** on conditions C0 to C4; no veto | a compiler in three stages, prototypes of (b), (i), (g) and (c), `build` over 1,323 program roots, instructions retired of the `heroes` process |
| ffi-pragmatist | **veto** (b), and (a) where it compiles every group's header together; **approve with conditions** (c) with (g) | C on this Mac and in Linux arm64 Docker (clang 21, clang 22, GNU ld), every ordered pair of 178 and 77 installed headers, nine Heroes programs |
| spec-warden | **veto** every sentence a one-unit route needs (U1, U1s, U2, U3, U4, D1); **object** to E1, C1, C3, O1; **approve** 0 tokens for each group compiled with its own header alone | `heroes measure`, eleven drafts, five probes: spec § 4's *Declaration order never matters* is false today at exit 0 |
| historian (advisory) | **object** to (b) and to any outcome leaving `clash` at exit 0; precedent for (c), a type comparison where a linker stands, and (d)'s true messages | C11 6.2.7p2, 6.7p4, 6.9p5; cfront 2.0 (1988); GCC LTO; llvm#56487; wasm-ld; MSVC C4744; Rust's `clashing_extern_declarations`; cgo; Zig; Nim #18776; Chromium's jumbo builds |
| blind seat | `fp`: 2 of 2 predict `test` exits 0 (it exits 1); `clash`: one predicts a refusal, one the wrong value as C's own behaviour; repair of `fp` from 538's message: 2 of 2 edit a correct program, one edit refused by `check`; `onedef`: 2 of 2 make the definition `static inline` | `llm-ergonomist-scoring.md`, corrected at 00:10 on the critic's finding 3 |
| critic, second pass | `fpat64` and `fprec` reproduce, `fpwide` is new; `skew` rules out the cheap form of C0; `--emit-c` means three things to three seats; the ffi-pragmatist's condition 2 refuses `fp`; (c)'s `test` without C2 makes `onedef` worse; (c) does not make § 4 true for one module | its own compiler in three stages, the cases under `202-critic2/cases/` |

## What the sitting measured

- **`build` is not per module today.** Its probes read every header of the
  program in one unit (the compiler-engineer; the critic reproduced it).
  `fpat64` (`fill(@x: i64)` over `void fill(int *x)`, beside `fp`'s two
  headers) builds at exit 0 and prints `6 4294967299`, because
  `selfhost/cli/pointee.hero:200-204` returns `ok()` when the dump unit does
  not compile; the same program without the conflict is refused
  `ffi_parameter_type`. `fprec` (a group record beside `fp`) is refused at
  `build`. `fpwide` (`fill(@x: i32)` over `void fill(long *x)`) builds and
  aborts 134 with advice naming the wrong repair. `skew` (a macro in one
  module's header choosing another module's record layout): no declaration
  of `S` builds in either `use` order, two of four messages contradict
  themselves, and `test`'s verdict flips with the order; the probe of the
  whole program compiles and answers wrong, so probing the whole program
  first and per module only when it fails is ruled out.
- **One C symbol declared two ways** (`clash`, defect 555) prints `6 4.0 10`
  at exit 0 in `build` and `run`; `clang -flto` on ld64 and lld tells
  nothing and prints `6 -0.000000` (the ffi-pragmatist). The C types of the
  two declarations, read from `-ast-dump=json`, are `long (long)` and `double
  (double)`, both external, while `fp`'s two definitions are `static` and
  are skipped (the compiler-engineer).
- **A non-static definition named by two modules** (`onedef`, `extdef`,
  defect 556) makes `build` exit 2, *duplicate symbol*, on both platforms;
  the same dump names it an external definition before the link. A
  tentative definition (`int counter;`) links on this Mac and is a multiple
  definition on Linux. Of the installed headers, 0 of 156 on this Mac and 0
  of 75 on Linux define an external symbol.
- **One unit for the whole program refuses real libraries.** Every ordered
  pair of installed headers compiled together: 18 cross-library pairs fail
  on this Mac (X11 and raylib, curses and raylib, `term.h` and six media
  libraries, libtasn1 and OpenSSL, oniguruma and two regex headers), 92 on
  Linux, 20 of them only in one order; every one builds today in two
  modules. Over the 1,323 tracked program roots, (b) and (i) move none, the
  tree holding no program whose headers fail together.
- **`test` and `--emit-c` compute other programs.** `macro3`: `build` 8,
  `test` 400. `cfg`: `build` `3 10`, `test` fails a correct test with `left:
  50 right: 10`, and the `--emit-c` file compiles to a program printing `3
  50`. `shim2` (two modules, each with the identical `static inline` shim
  design.md `:568` prescribes) is refused by `test`. `macro4`'s `test` tells
  a false `ffi_return_type`. Measured today, `--emit-c` exits 0 for `fp`,
  `fpat64` and `clash`, and clang refuses each file written.
- **Defect 550 on real libraries**: `rlx11` (raylib and X11) gets the old
  false message in `test`. `-fdiagnostics-show-note-include-stack` prints
  the include chain from the unit's line, in the same clang run, on clang 21
  and 22; built into `cli/header_refused.hero` (+13 / -4 code lines), both
  orders of `inc1`/`inc2` get the same true words.
- **538's note is false twice.** *Whichever comes first* is false on
  `rlmath2` and `tasn1b`, which pass `test` in the other order. Its first
  remedy led 2 of 2 blind readers to edit a correct program, one edit
  refused by `check` (`extern_across_modules`); both argued its second
  remedy cannot work for `fp`, unrun as a program.
- **One module is out of (c)'s reach.** `cfgone`, one module binding `a.h`
  (`#define LIMIT 100`) and `b.h` (`#ifndef LIMIT`, `#define LIMIT 10`,
  `cap`), prints `3 50` in one order of its two `extern` declarations and
  `3 10` in the other, at exit 0: spec § 4's *Declaration order never
  matters* (`:113`) is false today. `one` (538's own shape) is refused at
  `build` where the spec says nothing that refuses it.
- **A header-only C99 `inline`** (`c99`): `build` exits 1 telling
  `ffi_missing_link`, *no group says which library has it*, which is false,
  and `run` (at `-O2`) exits 0 printing `6`.

## Disagreements, stated plainly

- **Where `clash` is caught.** The ffi-pragmatist asks `check` to compare
  parameter types across groups; the critic showed that comparison refuses
  `fp`, whose groups bind `twice(x: i64)` and `twice(x: i32)` legally, and
  § 13 lets a Heroes type be wider than C's, so only clang's types decide.
  The compiler-engineer's C1 at `build`, on clang's dump, and the
  historian's *where a linker stands*, agree; the resolution takes them.
- **The spec-warden's 0 tokens** were given for each group compiled with its
  own header alone, route (f), which the compiler-engineer prices high (773
  tracked files with a group record to mirror, 220 `partial`, 112
  `constant`) and rejects. Under (c) one module's headers still share a unit,
  so the 0 holds for every program but `cfgone` and `one`, which R4 files.
  The spec-warden's vetoes rest on the budget and the thesis, and CLAUDE.md
  § 4 has seats veto on soundness; read as objections, and moot, since the
  resolution spends no spec token.
- **What `--emit-c` means under (c).** The spec-warden predicted its file
  prints each module's values, which one C file cannot do where two units
  conflict (C11 6.7p4); the compiler-engineer's C4 said panel 200 R1's
  compile catches `fp`, against its own prototype's row (it never compiled
  the file). The resolution chooses one file, refused truthfully where one
  file cannot mean the program, keeping panel 200 R1 and the seed's
  bootstrap (`seed/README.md:93`).
- **Which census counts.** The tracked tree has no pair that fails together
  (0 of 1,323); the installed headers have 18 and 92. The installed ones are
  the programs Heroes is for (design.md §1.11, everything comes from C), so
  the resolution counts them.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, the principle: every verb cuts a program's C by module, as `build`
   already says it does.** The probes, `build`, `run` and `test` compile one
   unit per module; `check` asks clang nothing, as today. Refused: (b), one
   unit for the whole program (18 and 92 real pairs, the ffi-pragmatist's
   veto); (a), a clang question in `check` (it refuses what (b) refuses);
   (d) alone (it leaves 555 and `fpat64` printing wrong values); (e), a
   comparison of Heroes types (it refuses `fp`; Rust's lint is the
   precedent, its false positives within a week); (h), LTO (clang told
   nothing, measured); (i), withdrawn by its author. design.md §4.1
   `:836-839`, *the whole program is still emitted as one .c*, stale since
   panel 093, is amended in the first landing. 0 spec tokens.
2. **R2, the first landing, batch 18, in this order, each step gated by its
   cases:**
   - **C0**: every probe per module, every time (`cli/pointee.hero`, the
     layout probe, `compiling.probe`, `artifact.hero:80`), in every verb;
     `pointee.hero:200-204`'s `ok()` on a dump unit that does not compile
     becomes a refusal of what it cannot read. Carried, the
     compiler-engineer's: about 7 probe runs instead of 1 on `selfhost/`,
     under 0.5% of a warm self-build, re-measured at the landing.
   - **C1**: every external declaration two or more modules' groups bind is
     compared on clang's types, from the same per-module dumps, and a
     disagreement is told at exit 1 before the link, naming both headers
     and both lines; a `static` definition is skipped. Whether it extends to
     an external name two modules' headers declare and no group binds is
     measured on the census at the landing, and taken where it refuses no
     tracked program.
   - **C2**: an external definition, a function's or an object's, a
     tentative one included, in a header two or more modules' units read,
     or two external definitions of one name, told at exit 1 before the
     link on every platform, naming the header and the modules, the note
     offering `static inline` or one module.
   - **(g)**: `-fdiagnostics-show-note-include-stack` in the compile and
     probe flags (`cli/flags.hero`; every cache key moves once, so `cache`
     alone at the gate), and 538's message naming the group's header the
     conflicting file arrives through, in both orders.
   - **538's note**: *whichever comes first* removed; the remedy *bind the
     two groups from two modules*, true on every shape measured; the second
     remedy removed until a program shows it works. A header that compiles
     alone and fails in `test`'s one unit (557's `macro`) is told as 538's
     class, naming the earlier header whose presence makes it fail, found by
     compiling the two alone, so the words are the same in both orders.
   - **The classes**: `.claude/rules/c-boundary.md` § A clang failure that the author's own
     extern caused (`:47` on) names 538's message
     as its seventh member (two headers that each compile alone, refused
     together) and C1 and C2 as its eighth and ninth, their codes the
     landing's. This sitting is their sitting (CLAUDE.md § 4).

   It closes 550, 555, 556, 557, 559 and 561; `test` still compiles one unit, telling true messages.
3. **R3, the second landing, after R2, in batch 18 if it builds and the next
   batch if not, its defect open until then:**
   - **`test` per module**, the test runner carried by `emit/unit.hero`'s
     root shim so the program's `main` does not run before the tests (the
     one-line prototype ran `main` before every test, `cases/fptest`).
     `fp`, `shim2`, `macro3` and `cfg` then pass `test` as they build
     (defect 560).
   - **`--emit-c` stays one file** (panel 200 R1 kept; the seed bootstraps
     from one file) **and is refused at exit 1, with a true message naming
     the two modules and their headers, where one file cannot mean the
     program `build` builds**: the fused file does not compile (R1's
     compile: `fp`, `clash`, `fpat64`), or a module's bindings mean
     something else in it than in their own unit (`cfg`, `macro3`). The
     second instrument is unbuilt: each binding's declaration, its C type
     and, for a `static inline` definition or a constant, its body or value,
     read from the dumps of the module's own unit and of the fused one, and
     compared. It lands only measured on the census (no tracked program
     refused, `selfhost/`'s `--emit-c` byte-identical); a comparison of
     preprocessed tokens is not it, since an include-guarded `typedef`
     moves between headers with their order (an inference, a question for
     the lane). 453 closes with it.
4. **R4, one module whose two headers' order changes what they mean, filed
   `blocking` now as defect 563** (spec § 4 false at exit 0, `cfgone`; `one` with it): (c)
   and (g) do not reach it. It goes to a sitting of its own once R3's
   instrument exists, since every route but a sentence needs it: a canonical
   header order (by bytes, as 538's message orders them; its cost the 107
   tracked files naming two or more headers in one module), R3's comparison
   run on the two orders and an order-dependent module refused, per-group
   units (f), or the spec-warden's E1 (+12 on the vendored maximum,
   objected).
5. **R5, the spec: no sentence.** U1, U1s, U2, U3, U4 and D1 refused with
   the one-unit routes they would describe; E1, C1, C2, C3 and O1 not
   adopted. 0 tokens, and the 333 the spec-warden measured spendable stay
   for panel 203.
6. **R6, filed now, with batch 18's round**, each `blocking` (a wrong value,
   a false message, a correct program refused): 559, the probes reading
   every header in one unit (`fpat64`, `fprec`, `fpwide`, `skew`, `macro4`'s
   false `ffi_return_type`); 560, `test` compiling another program than
   `build` (`fp`, `shim2`, `macro3`, `cfg`); 561, 538's note false
   (*whichever comes first*, `rlmath2`, `tasn1b`, and the remedy the readers
   followed into an edit `check` refuses); 562, a header-only C99 `inline`
   told `ffi_missing_link` falsely by `build` and run at exit 0 by `run`;
   563, `cfgone` (R4). The tentative definition joins 556 as a row of its
   cause.

**The conservative alternative, the author's to choose instead**: R2 alone,
`test` left one unit with true messages and `--emit-c` as today, R3's two
defects (`test`'s other program and 453) left open for a later sitting. **A
narrower stopgap**, the critic's, unrun: `pointee.hero:204` refusing instead
of `ok()`, which trades `fpat64`'s wrong value for refusing the correct
`fpat`.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the closing commit of the batch landing (c): `fpat64` builds at exit 1 `ffi_parameter_type`; `clash`, `onedef`, `extdef` exit 1 in `build`, `test`, `run`; `fp` exits 0 printing `6 8` in all three; the `selfhost/` diff at most 400 code lines | R3's landing |
| ffi-pragmatist | under (c), `rlmath1` and `rlmath2` both exit 0 printing `1.0 7` | R3's landing |
| spec-warden | P1: the spec stays 7,479 vendored and 9,847 real; `cfg`, `macro3`, `one`, `fp` print `build`'s values in every verb and both orders. **Falsified in advance for `cfgone` and `one`** by the critic (one module), which R4 files | R3's landing |
| critic | `skew` under C0: `skew32` builds in both orders, `skew64` is refused truthfully | R2's landing |

## Author's verdict

**RATIFIED, 2026-10-10**, R1 to R6 as written above, the author answering
through the question widget between 00:21 and 00:24 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R6* over
*only the first landing* (R2, `test` and `--emit-c` left as they are)
and *I want to read it first*, on the coordinator's summary of each route.
Recorded as a reading (CLAUDE.md § 4). The author may overturn it.
