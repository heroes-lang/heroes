# 016 — The corpus: many whole programs, all of them run

Closed 2026-08-13 · tag `m-program-corpus`

## The goal

The one milestone whose deliverable is **programs rather than compiler**. Its
warrant, from the ROADMAP: every instrument this project owns reports on the
programs it is given, and the M-generics-library audit had found two live defects
with *one* program — each reachable for three milestones with no test, *"because
nobody had written the program that meets them"*.

What it asked for: `examples/` reorganised as one directory per program; whole
programs of the kind other languages ship as a tour, each reading its input,
taking `args()`, ending with a status and asserting its own behaviour in `test`
blocks; all of them run in three configurations with the leak balance asserted;
error codes as `constant`s with a measurement; the Part 11 harness run over the
enlarged corpus; **the CI born here**, on Linux x86-64 as well as Darwin arm64;
and at least one program that does not end in `exit(code)`.

Every one of those landed. The corpus went from three program directories to
**nine**, `heroes mutate`'s corpus from 19 programs to 45, and its mutant count
from 1255 to 5798.

## What surprised

**A corpus is an instrument, and it reports on the compiler.** Seven programs
were written and **six compiler defects** came out of them — not one of which
was found by reading the compiler, and not one of which any existing test could
have found:

| # | what | how it presented |
|---|---|---|
| 1 | `heroes check` **hangs forever** on a variant case named `null` with any payload | no output, no exit, on the first program's first minute |
| 2 | `-x` on a name read as a negative **literal** | `x @ -x` on an `f64`: *expected `f64`, found `i64`*; `ok(-n)`: a **range** error about an expression with no value |
| 3 | a `constant` read **across a module** did not compile | *"`grid` names a module, not a value"*, on a line where it was followed by a dot |
| 4 | a dead temporary, `-Wunused-but-set-variable` on every build | a clang warning the golden corpus never saw |
| 5 | `heroes fmt` **changed the meaning of a program** | `(at / WORDS).to_str()` came back as `at / WORDS.to_str()` |
| 6 | the `@` marker unchecked on a built-in | `push(@lines, x)` said nothing about the `@` |

**Three of the six are one class, and it is CLAUDE.md §11's.** A narrowing that
rests on a premise about the world rather than on a fact about the value, whose
comment goes on reading as correct because the argument is still valid and only
the premise died:

- (2) `ExprKind::Unary { op: Neg, .. }` matched **flatly** in two of the three
  places that ask "is this a number literal", while the third — in the module
  whose doc says *"two readings of it would be two languages"* — recursed
  correctly;
- (4) a three-name list, `Call | Load | Const`, resting on the premise that
  nothing else could produce a value the emitted C never reads;
- (5) the number `7` for "a postfix receiver binds tighter than this", right when
  the precedence scale ended at `postfix = 7`, and left behind when panel 040
  added five bitwise levels and shifted everything above `Add` up by three.

(5) is the sharpest of the three, because the file **says so about itself**. The
scale's own comment reads: *"The same numbers as `syntax/expr.rs`'s table, and
they have to be: this decides where `heroes fmt` prints a parenthesis, so a scale
that disagreed with the parser's would print a program that parses differently
from the one it read."* It was true. It was read. It was agreed with. The
disagreement was twenty lines below it, in the same file, for one milestone.

**A prediction can name a live instrument and still be uncollectable.** Panel
046's R1 asks that a registered prediction name an instrument that exists on the
day of registration. Panel 040's prediction 2 did — `heroes mutate` has existed
since M-generics-library — and scoring it produced `held, vacuously`, because
**none of the twelve operators makes the substitution the prediction is about**.
R1 asks that the instrument exist; it does not ask that the instrument have an
arm for the question.

**A reader told a loop is quadratic, and given no non-loop, does the loop.** The
blind spec-only experiment that scored panel 037's clause reported the first half
held and the second half falsified, and the falsifier was not the wording: with
no lambda `map` cannot capture a local, and with no `repeat` there is no way to
make N copies of a character, so two of three tasks had no non-loop route at all.
The third — whose transform captures nothing — is the one program with no
accumulator in it.

**Two of the corpus's own test failures were the language being right.**
`n + left & 1` is `(n + left) & 1`, exactly as in C; and `mask & bit != 0` needs
no parentheses, exactly *unlike* C, where `&` binds looser than `!=` and that is
the most famous precedence wart there is. Both lines now sit together in
`examples/logs/mask.hero` with the pairing named.

## What broke and why

**The parser loop (1).** `case_block` recovered from a refused case name with
`skip_line`, which stops *before* an `Indent` by design so the next line is parsed
on its own merits. The refused case's payload block was therefore still under the
cursor on the next turn, fell into the same `_` arm, and `skip_line` returned
without moving. `field_block` twenty lines above has had its `Indent` arm since it
was written; nothing reached the case parser's because every existing test of a
refused case name used a case with **no payload**. One arm, `balanced_block()`.

**The formatter (5).** Five literal `7`s, replaced by two named constants,
`UNARY` and `POSTFIX`. The gallery could not have caught it: a gallery file is
asserted *canonical*, so a shape `fmt` mangles simply never got written into one.
`every_program_in_the_repository_survives_formatting_twice` caught it on the first
program in this repository to write `(a / b).to_str()`.

**The runtime cache raced.** Found by a test that failed **once, in one run out of
several** — the worst way for a defect to present itself. `runtime_object` checked
`is_file()` and then had clang write to that path, so the published flag went up
while the file was still half a file; `cargo test` runs its binaries in parallel
and the corpus harness made a third one that builds programs. It now compiles
beside the target and **renames**, which is atomic inside one directory. The full
suite passes from a cold `build/`, which is the state the CI will always be in.

**`heroes doctor` exits 1 on Linux**, because it asked `xcode-select -p` on every
platform. Found by writing the workflow rather than by running it.

**The measurement that went the way it was warned it would.** Panel 034 R4 asked
for error codes as `constant`s *and* named the awkward outcome in advance: the
operator may lose its sites rather than pass them. It did — `typo-code` went from
88 mutants at 0% to 3 at 0%, and `typo-ident` gained 127 sites at 100%. **85
sites moved from an operator that catches nothing to one that catches
everything.** The mistake class did not become detectable; it became a different
class, one the compiler already refuses.

Five of the new `constant` reads cross a module boundary — the exact shape panel
034 R4 named on 2026-08-12 as `store.ERR_UNKNOWN_ITEM` — and **that shape did not
compile until step 6 of this milestone**. The panel asked for a form the language
did not have, and nobody found out until a program needed it.

## What landed, and what carried forward

**M-program-corpus closed 2026-08-13, tag `m-program-corpus` — nine programs, six
compiler defects, and a CI on two platforms.**

    $ heroes run examples/maze/main.hero -- examples/maze/sample.maze
    S.#*******
    **#*#####*
    …
    steps: 25

`examples/` is one directory per program, found by looking rather than by list:
`tests/corpus.rs` takes any directory holding a `main.hero`, so a program joins
the suite by existing. Seven new ones — `json`, `markdown`, `spreadsheet`, `maze`,
`assembler`, `logs`, `todo`, `adventure` — each with `test` blocks, a
`main.expected` in the `run/` golden convention, and `main.args` feeding `args()`.
Six properties per directory, in three configurations, including *at least one
program that never calls `exit`* so the corpus keeps a path across the leak gate.

**542 tests · 51 CLI · 13 golden harnesses + 7 corpus properties**, clippy clean
under `-D warnings`, spec unchanged at **2974**, `mutate` **95% / 85% over 5798**
across 45 programs, determinism diff empty, PORT-DEBT unchanged.

Carried forward:

- **the CI has never run.** The workflow is in the tree and the first push starts
  GitHub Actions on this repository for the first time, which is the author's
  call. Everything it checks passes locally on Darwin arm64; **nothing has ever
  been run on Linux**, and the two ASan leak detectors have never been compared;
- **four spec clauses are unfunded** — `2588`, `2745`, `2768`, `2959`, 312 tokens
  — their predictions lapsed and each is in `DECIDE.md` to be re-argued under the
  removal branch. Three of the four are §1.0 compiler-need, so the likely answer
  is *keep, unfunded, and say so*;
- **three questions the corpus raised and did not answer**: a way to build N
  copies of a `str`, a way for `map` to see a local, and a `boolean-twin` operator
  for `heroes mutate` without which panel 040's own falsifier cannot fire;
- **`typo-digit` and `typo-code` both sit at 0%.** The first now has 12 sites
  where the §11 sweep found it 0; the second has 3, all in the gallery, kept
  literal on purpose so the operator keeps a place to fire.

Record: `docs/measurements/006`, `docs/measurements/007`, `docs/panel/037` and
`040` § Predictions scored. Next: **M-selfhost-probe**.
