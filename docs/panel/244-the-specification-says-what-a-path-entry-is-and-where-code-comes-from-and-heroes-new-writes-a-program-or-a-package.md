# 244: the specification says what a `path` entry is and where code comes from, and `heroes new` writes a program or a package

**Status** a full sitting on the specification's package text, convened 2026-11-10 by the
coordinator of batch 30 on the author's instruction of that day, meant as: *document packages well
on the site and in the guides, with the reasons behind each choice, and put what is missing into
the specification*; the author's second question of the day, a verb that writes a project's stub,
merged into it by the author's answer, meant as *merge the sittings*. The resolution is
`provisional — author ratification pending`.
**Seats** spec-warden, compiler-engineer, ffi-pragmatist, historian; the completeness critic on the
briefs before any seat (20 items, the briefs rewritten on them, the first versions kept as
`244-briefs/*.before-critic.md`) and on the reports after (18 items). No llm-ergonomist: its blind
reading is a paid run, not run; R1 below names what would need it.
**Base** `89db843472cf1c39450f4a2fc22ff99ff3d716f7` (`main`), its stage 2 compiler printing
`sources 14e9e5ce168a2336`; each seat built its own in `.claude/worktrees/244-<seat>`.
**Briefs** `docs/panel/244-briefs/`, **reports** `docs/panel/244-reports/`, **evidence**
`docs/panel/244-reports/evidence.tar.xz` (its note beside it).

## The proposal

Two questions (`244-briefs/00-shared.md`):

1. **The question.** Which of the rules the compiler holds of a package the specification states,
   in what words, at what cost, and which it leaves to the compiler's messages, by Principle 0 and
   design.md §1.6's budget: a sentence enters where a reader holding only the specification would
   otherwise write a wrong program or be unable to predict a refusal, measured, never because it is
   true. The candidates: `internal/` (panel 235's R16), where fetched code stands
   (`heroes.packages/`, its sum, `build` never fetching), the sources of an entry (index, `git`,
   `path`), `use <entry>` alone and `unknown_package_module`, the C boundary of a fetched package
   (panel 235's R9, C1 to C5), and task 2402's false sentence of § 13.
2. **Q-stub.** A `heroes` verb that writes a project's stub (issue 2441): its name (the author's
   `make` first), its operands in the project's vocabulary (*library* is a C library, spec § 13;
   Heroes code others publish is a *package*, spec § 1), where it writes, what it writes for a
   program and for a package, what it refuses, its exit codes, and what the site's verb count and
   the specification owe.

## The verdicts

| question | spec-warden | compiler-engineer | ffi-pragmatist | historian (advisory) |
|---|---|---|---|---|
| the question | **approve S1f** (d03, d07f, d08d), +46 / +47 vendored; object to every other draft; `internal` out while its shape is undecided | **object**, no veto: every rule a sentence would state is already in the compiler (0 lines); two rules are not yet one rule (a `path` in a package's catalogue; `internal` as a file) | **object** to any § 13 sentence on the C boundary of packages; the C5 refusal is the compiler's to close through feature 2358 | **approve**: state `internal` and correct § 1's `path` sentence; mechanics to the guide and the messages |
| Q-stub | the spec names no verb (+20 to +70 if it did); `program` and `package` fit, `lib` and `app` occur 0 times | built `heroes new program\|package <name> [-C]`, 180 code lines; the two-word operand puts `argv.hero` at 303 code lines against 300 | — | each of seven tools has such a verb, none named `make`; `new` writes a new folder (Cargo, Gleam); refuse an existing one and a name the language cannot import |

No veto falls on a route this resolution adopts.

## What each seat measured, in short

- **spec-warden** (`244-reports/spec-warden.md`): spec at 8167 legacy, 8314 cl100k, 10,898 real;
  § 1's *"names with its source, exact version and fingerprint"* is false for a `path` entry (a
  `path` entry written with version and sum is refused `catalogue_refused`); § 13's *"JSON ... comes
  from C libraries"* is false since `core` ships `encoding/json` (task 2402); six refusals already
  state the rule to the person who can act, so a sentence would charge every task for a round trip
  saved in a few. Found `b5` and `b10`, filed as defects 2442 and 2443.
- **compiler-engineer** (`244-reports/compiler-engineer.md`): sixty-odd probes; two dependencies
  calling different packages by one name work (`util`, `util_2`, prints `21`); a dependency's
  dependency used directly works (`2101`); a package's own catalogue holding a `path` is built by
  `check` and `run` and refused by `heroes add` (D2, filed as defect 2446). The Q-stub prototype
  writes `main.hero` for a program and a main module, a private module under `internal/` and a card
  for a package; what it wrote runs and tests clean, a program reaches the package by `heroes add
  --path` and prints `hello, ana`; it refuses an existing folder, `match`, `fn`, `my-lib`, `2d`,
  `a/b`, `.`, `..`, an empty name and an unknown kind at exit 2, writing nothing. Found D1 (a
  `.DS_Store` in a placed tree refuses every build).
- **ffi-pragmatist** (`244-reports/ffi-pragmatist.md`): of 18 constants of four real libraries
  fetched, 9 refused (`ffi_constant_type`), all 18 fine in the program's own tree; the refusal
  reaches the consumer and never the author; feature 2358's C compiles on clang 21 (unrun on 18).
- **historian** (`244-reports/historian.md`): Go's specification does not state `internal`, its go
  command's documentation does; source kinds, checksums and storage live in each tool's manual;
  Zig computes no hash for a `path` and Go omits the version of a local replacement; the seven
  project verbs and their refusals (§ F).
- **critic, second pass** (`244-reports/critic-reports.md`): item 1, the C5 refusal is by design,
  ratified with panel 235's R9; item 2, d07f is false for `cstr` and `ptr` constants, which build
  fetched, while the ffi-pragmatist's draft C is true on every shape run; item 4, a package fetched
  from an index whose own catalogue holds a `path` builds today, and editing the folder that `path`
  names moved the program's output from `2` to `3` with the package's sum unchanged; item 5, every
  price reproduced, and C2 (draft C in the example's own keys) costs +28 / +27; item 11, a program
  in its own git repository cannot read a `file://` index outside it (`source_refused`), so `core`
  has no measured route for a program outside this repository before ROADMAP row 78.

## The resolution adopted, provisionally

**R1, the question: the specification states three things, S1c2, and no other.**
- **§ 1, d03**: the catalogue *"names with its source, exact version and `sum`, or by a local
  `path` alone, which nothing verifies"*, so the sentence is true of all three sources and uses the
  key every message names;
- **§ 13's opening, d08d (task 2402), the named removal**: *"Anything beyond this document comes
  from packages or C libraries; threads, processes and files from the language's `hero_os.h`"*;
- **§ 13:428, C2**: *"A group's `constant` has no body: the header holds the value, in a package
  named by `version` or `git` never a float, a record or one read through an enumerator or
  object."*, which lives until feature 2358's landing removes it, and is not written if 2358 lands
  first.
The set reads 8201 legacy and 8349 cl100k vendored, +34 / +35, a lower bound; it lands in its own
commit after `heroes measure spec/heroes-spec.md --refresh`, a call the author is asked for, its
output pinned in `tests/harness/suite_spec.hero` and `selfhost/measure/pinned.hero` with the
ledger's row (`.claude/rules/spec-shape.md`). **`internal` (I2, +35 / +37) enters only if the blind
reading pays for it**, a paid run the author is asked for; until then the compiler's message,
since defect 2439 stating the element rule in one sentence, carries it. Out: `heroes fetch` and
`heroes.packages/`, the stand-in and outside-reading clauses, `use <entry>`,
`unknown_package_module`; the `index` line stays out now that 2442 and 2443 make its messages true.

**R2, Q-stub: `heroes new`.** `heroes new <name>` writes a program, `heroes new <name> --package` a
package, `-C <folder>` as `add` and `fetch` take it; one word for the operand, so `argv.hero` stays
under its ceiling. A program is `main.hero` alone: no catalogue, no ignore file, no `git init`. A
package is its face module with its tests, and `<name>/internal/<x>.hero`; no card until task
2294's writer is a verb, since `examples/packages/README.md` § 8 forbids a card written by hand.
Its `next:` line names `heroes add --path` and says a fetch may refuse what a `path` build accepts.
It refuses, at exit 2 and before writing anything, an existing folder or file, a name holding a
`/`, `.` or `..`, and for a package a name no `use` can write (a keyword, a name outside the
module-name set); it writes into a staged folder renamed into place, so a failure leaves nothing.
The specification does not name it. Feature 2441; the verbs' count moves from 15 to 16
(`selfhost/cli/table.hero:263`, `argv.hero`'s two tests of the command list, the site's
`claims.ts` and both start pages), and `.claude/rules/cli-surface.md`'s catalogue of verbs takes it
in the coordinator's commit, citing this sitting as the author's decision under the stopping rule.

**R3, defect 2446: one rule for the build and the verbs.** A `path` is refused in the catalogue of
a package named by `version` or `git`, by the build as by `heroes add` and `heroes fetch`, since
that package's tree is fingerprinted and a `path` escapes the fingerprint (the critic's item 4); a
`path` is accepted in a package that is itself a `path` entry of the program, matched by identity
(panel 235's R3), both folders being the author's own (R6), and `heroes add --path` writes the
matching entry; and the author's `path` build notes that a fetch of that package would be refused.

**R4, what is filed beside it.** The C5 refusal's note, *"Write the value so in the package's
header"*, has no form that works for a float or a record (the critic's item 3), and with the
`shadowed_header` note of a package's folder it tells the consumer to edit files they are refused
(the ffi-pragmatist's first defect): defect 2458, `blocking`. D1, a `.DS_Store` written into a
placed tree: defect 2459, `adjacent`. Feature 2460: a `path` build notes every refusal a fetched
build of the same package will make (C5, the stand-in, R3), so the refusal reaches the author on
the author's own route (the critic's question 16, the spec-warden's item 6). Defect 2446 gains the
critic's index shape, as a comment. `selfhost/module/private.hero:1`'s *"spec § 1"* is corrected to
panel 235's R16.

**R5, the landing.** R1 by the coordinator in batch 30's round, after the author's yes for the
refresh; R2 in a lane of batch 30, `b30-new`; R3 in lane b30-pkg after its queue; 2458 in lane
b30-shadow, which holds `shadowed_words.hero`, and the citation in lane b30-pkg, both in batch 30;
2459 and 2460 for the next.

## Disagreements, not smoothed

- **Is the C5 refusal by design?** The ffi-pragmatist reads it as the compiler's error (§ 13:428,
  *"the header holds the value"*, and CLAUDE.md § 12), and would write no clause and make 2358
  `blocking` before the tag. The spec-warden and the critic read it as by design, and the critic
  ran why: panel 235's R9 C5 was ratified with its refusal, and panel 237's R7, the precedent the
  ffi-pragmatist cites, amended the specification and kept the compiler's refusal. R1 takes the
  reading by design and states it; **the ffi-pragmatist's route is recorded** for the author.
- **d07f against C2.** The spec-warden's d07f says a fetched package's `constant` is an integer or
  `bool`; the critic built a fetched `cstr` and a fetched `ptr` at exit 0. C2 is true on every shape
  run and costs 12 vendored tokens less.
- **`internal`.** The historian approves stating it; the spec-warden objected because its shape was
  undecided, which defect 2439 has since settled in one sentence; no seat named a payment. R1 puts
  it to the blind reading rather than in or out by opinion; **the conservative alternative is S0**,
  d03 and d08d alone (+6 / +8), with the C5 refusal left to its code.
- **Q-stub's operand.** The compiler-engineer built `new program|package <name>` and measured that
  the second word breaks `argv.hero`'s ceiling; R2 takes one word and a flag. **The conservative
  alternative** writes a program only, until the card, 2446 and the critic's question 16 are
  settled.
- **R3.** **The conservative alternative** refuses every `path` in any package's catalogue at
  build, which leaves no route for two packages developed side by side and narrows R6 with no
  ruling behind it. Accepting a `path` everywhere, with the verb following the compiler, is refuted
  by the critic's item 4.

## Prior art, per resolution

R1's `path` sentence rests on Zig's and Go's local sources, reached first; R2 on Cargo's, Dart's and
Gleam's project verbs, reached first. R3 rests on none. Every page was re-read on 2026-11-10 by the
coordinator's `curl` of the page or its raw source, each sentence found verbatim; Zig's at
Codeberg's 0.16.0 tag, its GitHub repository marked moved (the historian's correction). The lines
are the decision issue's `## Prior art`.

## Left open, as questions

- Where a package's author learns what a fetched build will refuse (the critic's question 16):
  feature 2460 is the answer proposed.
- Is feature 2358 inside C5, its C11 route not being "clang's dump" (the critic's question 17)?
- § 1's example names `heroes add json`, a package no index serves, and after d08d nothing in the
  specification says where JSON comes from; K1, naming `core`'s modules (+16 / +16), was priced by
  the critic and proposed by no seat.
- A program in its own git repository has no measured route to `core` before M-package-index
  (ROADMAP row 78): `add --index file://../idx` is `source_refused` there.

## Predictions to score

- **compiler-engineer**: at `b30-new`'s commit, `git diff --numstat -- selfhost` shows 150 to 350
  added lines and none under `check/`, `ir/`, `emit/` or `runtime/`; `argv.hero` stays at or under
  300 code lines with a one-word operand (its other half, a two-word operand, is not built and is
  not scored).
- **ffi-pragmatist**: feature 2358, built as its report describes, makes the fetched `hmath`,
  `hcurl` and `hray` of `p244-scratch/ffi-pragmatist/e4/idx` print their own trees' values, keeps
  `1 << 31` and `P_BAD` refused, and a header constructor writes no file during a cold build;
  checkable at 2358's landing, clang 18 unrun.
- **spec-warden**: W, S1f read exactly at 8213 / 8361 vendored and 10,940 to 10,975 real, belongs
  to a set not adopted; S1c2's real count is read at R1's landing and written beside it.

## Process notes

- The seats were three by the brief's first line and four as the sitting ran: the ffi-pragmatist
  and the historian were briefed beside the spec-warden and the compiler-engineer before any seat
  started; the critic read all four briefs first.
- Q-stub entered after the spec-warden and the historian had started, by the author's answer; both
  were sent the question and answered it in their reports; the compiler-engineer's brief carried it
  before it started.
- The compiler-engineer and the ffi-pragmatist stopped on the account's rate limit at about 20:05
  and were resumed on another account at 20:14, from their reports.
- The critic's second pass had one command refused by a safety check, an `rm -rf *` after a `cd`
  into a folder it had just made; nothing was removed, and it re-ran the probe in a fresh folder.
- The compiler-engineer's worktree holds the Q-stub prototype uncommitted, `stub.hero` marked
  intent-to-add in that worktree's own index; its diff is
  `p244-scratch/compiler-engineer/qstub/qstub.diff`.

## Author's verdict

Pending. The sitting's decision issue, `Panel 244:`, is filed open:
https://github.com/heroes-lang/heroes/issues/2461. **What a yes settles**: R1 to
R5 as adopted, the specification amended by S1c2 in its own commit after the refresh, `heroes new`
as the sixteenth verb by the author's decision, defect 2446's rule, R4's filings. **What it does
not**: `internal` in the specification, which waits on the blind reading; the predictions above, on
their own clocks; the conservative readings, which stay the author's to choose.
