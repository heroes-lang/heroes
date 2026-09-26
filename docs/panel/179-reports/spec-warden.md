# Panel 179, spec-warden report

Written 2026-09-27 at 00:20 in `<scratchpad>/179-spec-warden/`, a copy of lane g
at `83ac68c1`, with a compiler built there from the seed (`clang -O2 -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, `heroes 0.2.0`). Every number names
its command. Load at the sitting: `uptime` read 8.16 on 8 cores, so nothing here
is timed; the one timing quoted is CARRIED and says so.

- `verdict`: **approve**, with the shape and two conditions below. No veto ground
  exists: the measured budget is 8861 real against 10240 and the proposal touches
  no spec sentence.
- `section`: design.md §1.6 (the budget: delta zero, measured), §1.2 (the reader's
  cost: 24 help tokens), §3.5 and §3.3 (one command, the compiler as a library),
  with the stopping rule at `.claude/rules/cli-surface.md` (panel 016).
- `spec_token_delta`: **measured, 8861 real, 6693 vendored maximum, delta 0.**
  `./heroes measure spec/heroes-spec.md`: claude-legacy 6568, cl100k_base 6693,
  maximum 6693, spread 125, real 8861 (`claude-opus-5`, pinned 2026-09-25, not
  reported STALE, so the pin matches this content); headroom 1379, of which the
  FFI floor mortgages 60. `grep -c fmt spec/heroes-spec.md` reads **0**, and
  `grep -n -iE 'heroes [a-z]|formatt|subcommand|--[a-z]'` hits only line 3
  (`Heroes is a small compiled language`, a false hit): the spec names no tool at
  all. So neither spelling, flag or subcommand, owes the spec a sentence. The
  brief's premise that § 12 names `fmt` is false on this copy: § 12 is *Tests and
  holes* (`grep -n '^## ' spec/heroes-spec.md`). After the landing: the same
  8861, scored as the prediction below.
- `removal`: nothing from the spec, and that is not a problem here: §1.6's payment
  rule prices spec tokens and the delta is zero, so nothing is owed. From the
  process, what comes out is the 868 lines of throwaway Python across 14 files
  the seats wrote (`wc -l` over `heroes-recovery-2026-09-26/skeptic-g4` and
  `skeptic-g5`), which CLAUDE.md § 10 and design.md §3.5 refuse as a script
  anyway. The 42 `surface` rows stay: they are goldens with expected text, and the
  probe asserts invariants, not text.
- `needed_for_self_hosting`: **no**. The compiler compiles itself without it.
- `argument`: Spec delta is a measured zero: `grep -c fmt spec/heroes-spec.md`
  reads 0 at 83ac68c1, so no spelling owes §1.6 a token; the brief's premise that
  § 12 names `fmt` is false (§ 12 is Tests and holes). The golden harness types
  `fmt` (`suite_canonical.hero:110`; 42 `surface` rows), and those rows caught
  none of defects 096 to 101 while the generators caught six (carried): the
  stopping rule's burden, met. Same input, so a flag: 24 help tokens against a
  verb's 47 to 49, and a verb needs a proven overload nobody measured. What I
  refuse is an unpinned count: over the 30 `comments101` fixtures my compiler
  makes 7982 variants, 6350 parsing, and the gate pins that floor or a shrinking
  generator stays green.
- `prediction`: three numbers, all with instruments that exist today, scored on
  the landing commit and again at the M-agreed-retention close. (1)
  `./heroes measure spec/heroes-spec.md` reads real **8861** and vendored maximum
  **6693**, unchanged. (2) `./heroes --help | wc -l` reads **72** (71 today plus
  one flag line) and `./heroes measure` over the saved help text reads a vendored
  maximum of **925 ± 3** (901 today plus the 24 of a one-line flag); a verb would
  read 74 lines and 948 to 950, which is how the shape is checked after the fact.
  (3) The probe over `tests/golden/surface-fixtures/comments101/` at the landing
  reports **7982** generated and **6350** parsing with the 30 fixtures unchanged;
  if the lane's fifth repair round changes the fixtures the numbers move, which is
  why the floor is pinned at landing and not tonight.
- `condition`: my verdict becomes **object** if any of these holds at the
  landing: the probe spawns `heroes` per variant instead of calling the formatter
  and its guard as §3.3's library (the Python did 6 spawns per variant, two
  `fmt`, two `parse --dump-ast`, two `lex --json`, which is 38,100 for the
  fixtures alone); the harness row carries no count floor; a top-level verb lands
  without a measured overload of `fmt`; `--probe` and `--in-place` can be
  combined without a refusal. It becomes **veto** only if the spec's real row
  moves above 10240, which nothing in the proposal touches.

## The brief's four questions

**1. Spec tokens.** Measured above: 8861 real, 6693 vendored maximum, delta 0.
No spelling owes a sentence, because the spec is the language and names no tool:
zero hits for `fmt`, for `heroes <verb>`, for any `--flag`. If a future sitting
put the tool surface into the spec that would be a §1.6 change of its own, with
its own payment; this proposal is not it.

**2. Principle 0's burden and the stopping rule.** Principle 0 (CLAUDE.md § 2)
governs what enters the LANGUAGE; the tool surface's analogue is panel 016's
stopping rule, and the brief asks me to apply it. Which of the three types the
probe: **the golden harness, and it types `fmt` already.**
`tests/harness/suite_canonical.hero:110` runs `[compiler, "fmt", path]` over the
tree and compares against the file; `tests/harness/suite_surface.hero` carries 42
rows whose argv starts with `fmt` (`grep -c 'argv: "fmt'`), each a fixture under
`surface-fixtures/comments101/` with its expected text. The measured argument is
the shared brief's carried one: those rows caught none of defects 096 to 101, and
the three generators caught all six. That is CLAUDE.md § RUN IT's rule, *a repair
is attacked at the shapes next to the one that provoked it*, made mechanical for
one tool. The shape follows from the rule's own sentence: the input is the same
`.hero` file and the question is the same one `fmt`'s guard already asks (parse,
fixpoint, tree, anchors), widened to the file's neighbours, so it is a **flag on
`fmt`**. A subcommand answers a different artifact class, and a new verb needs a
proven overload of an existing one; `fmt` carries one flag today (`--in-place`,
`selfhost/cli/table.hero:106`) and nobody has measured an overload. `--emit-c`
is the precedent that a flag may change what is printed about the same input
(`.claude/rules/cli-surface.md`: *an output, not a dump*); `mutate` is the
precedent for a verb, and it is a verb because it is Part 11 metric 3, which the
probe is not. What would make a verb right: a measured overload, for instance a
second and third probe flag on `fmt` that no longer read as one question. Record
for the author: the verb spelling costs 47 to 49 help tokens and 3 lines against
the flag's 24 and 1.

**3. The reader's cost.** `./heroes --help > help.txt; wc -l` reads **71**
lines; `./heroes measure help.txt` reads claude-legacy 844, cl100k_base **901**,
spread 57, no real row (the file has no pin). A candidate flag line written to
`flag.txt` (`      --probe  generate every comment and bracket variant of the
file, format each, and report the first that breaks`) measures **24** on both
vendored tables; a candidate verb block of three lines (`probe <file.hero>`, its
summary, an `--all` flag) measures **47 / 49**. These are candidate texts, priced
on the vendored instrument in my copy, not the landing's text. Under §1.2 the
number the formula governs is unchanged: the spec reader's program tokens and
rewrite rate do not move, because the probe adds nothing a program can contain.
What it moves is the FORMATTER's silent rewrite: a comment moved to another
owner is a rewrite the reader never sees and pays for later, and defects 096 to
101 are six of them. One correction round is 500 to 2000 tokens (§1.2); the
probe's cost to every reader is 24 tokens of help, once.

**4. The home of the probe's results.** Counted with my compiler over the 30
`.hero` files of `comments101/` (`ls *.hero | wc -l`; the directory's 31st entry
is `README.md`; the brief's *38 fixtures* I could not locate: the five comment
fixture directories hold 41 by `find`): comment insertion 3347 generated, 3347
parsing; bracket breaks 4077 generated, 2802 parsing; parenthesised values 558
generated, 201 parsing; **7982 generated, 6350 parsing** (`gen/parsecount2.py`
in my directory, the seats' generators with `rd.TRUNK` pointed at my binary).
The whole tree is 886 files (the brief's count, not re-run here) and the carried
totals are about 148,000 variants. The brief's *a probe over the fixtures costs
seconds* is **unrun**: per-variant time was not timed tonight (load 8.16), and
the one carried figure, `fmt` on `selfhost/check/walk.hero` at 0.29 to 0.31 s
user, is for a 2292-line file (`wc -l`), not a fixture. What decides seconds
against minutes is process spawns: the Python paid 6 per variant. So the gate
form runs the formatter and its guard in-process (§3.3) over the fixture
directories, and the row asserts three things or it is the *green run that
tested nothing* `.claude/rules/verification.md` warns of: exit 0, the printed
`generated N parsing M` at or above a floor pinned in the suite the way
`SPEC_TOKENS` is pinned (7982 and 6350 for `comments101/` at the landing), and
the first failing variant's text on a red. The whole-tree form is the same flag
over a directory operand, as `mutate [file]` defaults to `examples/`, run by hand
before a push that touches `selfhost/print/` with its counts recorded in the
milestone's journal; it does not enter the net. No nightly exists to receive it:
`grep -n -iE 'schedule|cron' .github/workflows/*.yml` reads 0 hits, and adding a
CI leg is outward-facing and the author's to ask for.

## What I searched and did not find, as questions

- A prior sitting on a formatter variant probe: `ls docs/panel | grep -iE
  'fmt|format|probe|comment|surface'` names 016, 095 and 151; 095 is the
  non-fixpoint order of work and 151 is §4.19's FFI probe, a different thing. Is
  there a ruling under another name?
- The brief's 38 fixtures: 30 in `comments101/`, 41 across the five comment
  fixture directories. Which set was meant?
- design.md covers the budget (§1.6), the reader's cost (§1.2) and the one
  command (§3.5); it does not say where a harness pins a generator's count, so
  that objection stands on `.claude/rules/verification.md`'s warning and on
  `suite_spec.hero`'s pinning shape, not on a design.md section.
