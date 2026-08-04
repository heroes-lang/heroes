# Panel 016 — the `heroes` command surface

Date: 2026-08-04. Trigger: CLAUDE.md §4, architecture (**tool surface**), and an
author instruction — *"comincia a pensare all'ergonomia del comando heroes per non
far sì che diventi disordinato"*. Convened **before** the flags arrive rather than
after: M3d adds `--json` and `--permissive`, M4 `--dump-ir`, M5a `--emit-c`/`-o`,
M6 `test`/`outline`/`explain`, M7 `cc`. CLAUDE.md §10 forbids a second binary, a
script and a Makefile, so every capability this project ever grows becomes a
subcommand or a flag of this one command. The surface is a design artifact with a
budget, exactly like the spec — and nobody had ever priced it.

Three judges. The ffi-pragmatist has no standing (no C, no ABI); the spec-warden's
instrument measures the *spec*, and the help text is not the spec — its §1.6-style
question is recorded in the watch list instead.

## The surface as it was

```
heroes doctor · lex <f> [--json] · measure [file] · parse <f> [--dump-ast]
       check <f> [--dump-scopes] · fmt <f> [--write] · --version
```

Six commands, five flags, **494 lines and zero tests** — of which **112 lines were
duplicated argv parsing** (four commands × 25 lines, plus `measure`'s 12), six
`USAGE` strings, five copies of `"unexpected argument"`, five of `"cannot read"`,
and 25 `ExitCode::FAILURE` sites meaning two different things.

## Verdicts

| Judge | P1 one inspection spelling | P2 `--json` = machine-readable | P3 `--in-place` | P4 exit codes | P5 one parser |
|---|---|---|---|---|---|
| compiler-engineer | **VETO** merging `c` into `--dump`; object to the rest | approve | approve | approve, with a condition on the harness | approve, with an `enum Tag` condition |
| llm-ergonomist | approve | **approve, blocking** | **approve, blocking** | **approve, highest value** | **approve, blocking, must be strict** |
| historian (advisory) | approve the flag *form*; object to merging `c` | approve | **object** — a rename without a sourced benefit | approve | approve the mechanism |

Measured:

- **the guess ledger** (ergonomist, from the help text alone, five tasks): **nine
  guesses, four of them silent** — including `fmt --write` read as "write the
  output somewhere", which destroys a file while exiting 0. Under P1–P5: **two
  guesses, one silent**, and the survivor is a milestone artifact (`check` is not
  `build`), not a naming choice.
- **the cost** (engineer, prototyped and compiled): the table-driven parser is
  **+101 lines now** (595 vs 494), repaying at about the eleventh command, i.e.
  M6–M7. The 14-command table measures 124 lines, ≈7.6 lines per command against
  26 today, and `cli.rs` stays constant — so it is under CLAUDE.md §11's limit to
  ~28 commands.
- **the blast radius of the rename** (engineer): **6 hits**, all in-repo, none in
  `editors/`, `site/`, `spec/`, `harness/` or `tests/`.

## What changed the proposal

**1. P1 was half-decided already, and the code had drifted from it.** design.md
§3.5 says, in the document's own words: *"Inspection is flags (`--dump-tokens`,
`--dump-ast`, `--dump-ir`, `--emit-c`, `--emit-asm`, `--pipeline`), **not
subcommands**"*. So `heroes dump ast` was never on the table, and the spellings
were already chosen — the code simply had `lex --json` printing tokens by default
instead of `lex --dump-tokens`. The proposal's `--dump=<stage>` is therefore not a
fix but a *second* change, and the engineer measured the value at almost nothing:
each command has exactly one legal stage (`lex`→tokens, `parse`→ast,
`check`→scopes, `build`→ir), so the stage is fully determined by the command name.
Adopted: **§3.5's hyphenated spellings, verbatim**, plus the one thing the
ergonomist said the `=` form was actually worth — **an error that enumerates**.

**2. `--emit-c` is not a dump, and the veto is precedent-backed.** `--emit-c`
produces the artifact clang then compiles, and CLAUDE.md §7's double-emit
determinism diff pins its bytes; a `--dump=*` value is unstable human text. The
historian found that no shipped compiler merges the two: rustc splits stable
`--emit=` (artifacts, `-o`-able) from unstable `-Zunpretty=` (dumps, printed,
never stabilised in nine years), and Swift's dump flags *halt* compilation, which
is why `-save-sil`/`-save-ir` had to be added separately for the case that does
not. `--emit-c` stays outside the `--dump-*` family, and the ROADMAP says so.

**3. P3 splits the judges, and the ergonomist's veto decides it.** The historian
is right that no formatter has documented a `--write` → `--in-place` rename, and
that the *sourced* remedy for the danger is gofmt's (keep the name, add an
automatic backup). But the ergonomist's finding is not about precedent: from the
help line alone, "write" is read as "write the output somewhere" — the
non-destructive meaning — so **the most plausible misreading is the one that
overwrites the file**, silently, exiting 0. That is a non-local construct by its
mandate, and it holds the veto. Adopted with the historian's objection recorded:
`--in-place`, and the retired spelling names its replacement, which is the same
treatment the *language* gives `fn`.

**4. P4 is not ceremony, and the number 2 is POSIX's.** `grep` and `diff` are
specified as 0 / 1 / >1-is-an-error; `git diff --exit-code` copies them; **javac
has shipped `OK(0) · ERROR(1) · CMDERR(2) · SYSERR(3)` since JDK 1.x**; ruff
documents 0/1/2 and says it aligns with ESLint, Prettier and RuboCop. The one
counter-precedent is real and recorded: **tsc's 2 means "errors, output emitted
anyway"**. Adopted: 0 clean · 1 the input has diagnostics · 2 the tool could not
run, printed in `--help` because a wrapper has nowhere else to read it, and the
golden harness now asserts `code() == Some(1)` rather than merely non-zero.

**5. P5 ships strict, or not at all.** The ergonomist's condition: a *lenient*
shared parser reintroduces the worst silent failure permanently — `check --json`
accepted and ignored, prose fed to a JSON reader. The engineer's condition: an
`enum Tag` rather than a name string, so a table entry with no dispatch arm is a
compile error. Both are in. And the CLI crate has tests for the first time: ten,
covering the contract rather than any one command's wording.

**6. The stopping rule is sourceable, and it is not "refuse to grow".** The
historian found that the tools which held the line did three things: a
**namespace for the long tail** (`go tool`, `crystal tool` — top levels of 19 and
12), an **external extension point** (cargo's `cargo-<cmd>` on `$PATH`, which
CLAUDE.md §10 forbids Heroes), and a **willingness to remove and re-add** (deno
bundle, removed in 2.0, restored in 2.4 once defensible). And in every recorded
case the trigger for a *new verb* was a proven overload, never a new capability:
`git checkout` → `switch`/`restore`, `go get` → `go install`. Adopted into
CLAUDE.md §10, in the engineer's wording joined to the historian's.

## Resolution — RATIFIED 2026-08-04

Ratified as it lands, by author instruction (`/goal`).

1. **§3.5's spellings**, implemented: `lex --dump-tokens`, `parse --dump-ast`,
   `check --dump-scopes`, and `--dump-ir` reserved for M4. Every stage prints
   **only when asked** — `lex`'s print-by-default is gone.
2. **`--json` says how, never what**: it is the format modifier of whatever the
   command already prints, on every command that prints anything structured, and
   at M3d it covers `check`'s *diagnostics*, which is the case the ergonomist
   said must be named out loud.
3. **`--in-place`**, with `--write` and `-w` retired and naming it.
4. **0 / 1 / 2**, printed in `--help`, asserted in the harness.
5. **One strict table-driven parser** with `enum Tag`, an enumerating
   unknown-flag error, and ten tests.
6. **`--emit-c` stays an output**, outside the `--dump-*` family.
7. **`measure [file]` states its default** and stops citing `§1.6` in the terminal.

## Predictions to score

| # | Judge | Prediction | Checkable at |
|---|---|---|---|
| 1 | compiler-engineer | with P5 landed, `grep -rc "unexpected argument" crates/heroes-cli/src` returns exactly **1**, and the crate measures ≤780 lines at M5a close; under the status quo the grep returns ≥7 and the tree ≥700 | M5a — **first half scored now: 1** |
| 2 | compiler-engineer | a uniform `--dump=<stage>` including `c` would not survive M5a unmodified: it would need `--emit=c` split back out, a special case inside the handler, or a `--dump=c` that ignores `-o` | M5a (moot under the adopted resolution; kept as the reason it was refused) |
| 3 | llm-ergonomist | guesses per five-task set **9 → 2**, silent-failure class **4 → 1**, first-try-correct command lines **≈55% → ≈90%**, silent wrong outcomes **≈20% → ≈2%** | first Part 11 run |
| 4 | llm-ergonomist | an injection set of 10 plausible-wrong command lines scores **0 of 10** accepted under a strict parser; today 1–3 are accepted with plausible output | now — the surface tests are that injection set in miniature |
| 5 | historian | at M5a, if `--emit-c` had been merged into `--dump`, one of three specific repairs would appear in the commit that lands C emission | M5a |

## Watch list

- **`--no-line` is refused by the engineer's own stopping rule**, and CLAUDE.md §7
  currently mandates it: design.md:469 says it "exists for debugging the emitter
  itself" — the author's activity, not the tool's capability — and it doubles the
  emitter's output space while the determinism diff is green in only one mode. The
  proposed replacement is three lines in the emitter's test helper. §7 is amended
  to say so, at M5a, where the flag would otherwise be born.
- **The help text has no budget**, and the historian searched: GNU says "brief"
  with no number, git tiers its commands instead, zig's help is a hand-written
  constant. On this Heroes is on its own; on the *mechanism* it is not — generate
  the help from the parse table (ripgrep), never the parse table from the help
  (docopt, unreleased since 2014).
- **`check` is not `build`.** The last silent guess in the ergonomist's ledger:
  asked to "check whether app.hero compiles", a model runs `heroes check` and
  reports success for a program that has no backend yet. Nothing in this panel
  fixes it; it wants a word in the help text at M5a.
- **`-` as stdin, `--` as the end of flags, and flags before the subcommand** are
  all unsupported and undocumented. Rejections, so relaxable.
