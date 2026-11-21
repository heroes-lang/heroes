# Panel 253, spec-warden

Read `00-shared.md` first, whole: the frozen commit (`181555e9a32d0b18878f9f10e48d2eadd939c285`), the six
questions as spec diffs with their conservative alternatives, the measurements and the rules. You judge the
indicator (design.md §1.6, `grep -n '^### 1.6' docs/design.md`: line 253; the payment rule from line 324,
*"Every amendment owes a named removal or a registered prediction"*; the consolidation block's 1,500 real
tokens, line 292) and Principle 0's burden (CLAUDE.md § 2): for each question, whether the specification
must say anything, where, at what price, and what pays for it. Veto on a budget breach or an unmet burden.

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-spec-warden/`,
made by you as `00-shared.md` § Where each seat works shows; binaries `sw-...`. **Your report** is
`.claude/worktrees/rebel-round/docs/panel/253-reports/spec-warden.md`, written as you go, in
`.claude/agents/spec-warden.md`'s structure, **one block per question**: 1, 2, 4, 5, 6 (3 adds no sentence
by the brief's reading; say so in one line, or say what it owes). Read `.claude/rules/spec-shape.md` in
your copy. No paid run but the refreshes below, no credential printed, no `heroes test`.

## FIRST: your registered predictions, before the blind run

**The coordinator starts the blind seat's runner only after your report holds your registered
predictions, and the runner checks it**: `run.sh` (`.claude/worktrees/rebel-scratch/253-llm-ergonomist/run.sh:61-76`)
refuses to start unless your report holds, on a line of its own, exactly

```
## Predictions registered before the blind run
```

**with at least one non-empty line under it before your next `## ` heading** (a heading written first with
nothing under it is refused; the critic's finding 6), and it **copies your report as it stands then** to
`253-llm-ergonomist/warden-at-start.md` and logs that copy's sha256: **the rules score only the predictions
in that copy**, so a prediction you add later, even under the same heading, scores nothing.
Panel 251's were written 13 seconds after its runner started (`docs/panel/251-reports/critic-reports.md`,
finding 4), which left its sentence R paid by a prediction the synthesis had to admit as a departure. So:
**write that heading and your predictions under it first**, worded in the counts the blind seat's
registered rules score (`llm-ergonomist.md` § The decision rules, as repaired after the critic's first
reading: task t2 for Q2's sentence, its `now` cell now a reader's default; t4 for Q4's, its `now` cell the
count of silent passes; t6 for Q6's, over `yaml`, `template` and `time` at `0.4.0`; five sessions a cell),
**then** tell the coordinator in your report's first line that they are
written; never read anything under the blind area before the runner has finished. A prediction you add
after the runner starts is written under another heading and scores nothing.

## The count before and after, as measured

The spec did not move since 2026-11-15: 492 lines, 29,407 bytes, sha256
`1d532d514831aa70234ef0eb8ad77850ebd4c2e390234f22e973a363ed757001`. By the writer's stage 2 (`sources
23c7d2a626b29fd5`) in its copy, `./heroes-next measure spec/heroes-spec.md` (`bw/measure-before.txt`, `bw/`
being `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-briefwriter/`):
`claude-legacy 8551`, `cl100k_base 8702`, maximum 8702, **`real 11384`** (*claude-opus-5, 2026-11-15*, the
pin in `selfhost/measure/pinned.hero`, `SPEC_REAL_TOKENS` at its line 54, **carried**: no refresh ran),
headroom 5000 against 16384, of which 60 the FFI floor's (panel 030 R3).

**After**: each arm written by `bw/tools/make_arms.py` (every edit asserted to match once) into
`bw/arms/`, its diff beside it, counted from the copy's root (`cd bw/tree && ./heroes-next measure
../arms/<file>`, `bw/measure-arms.txt`), **vendored, a lower bound**:

| arm | the edit | sha256, first 16 | lines | legacy | cl100k | maximum | delta |
|---|---|---|---|---|---|---|---|
| `spec-now.md` | none | `1d532d514831aa70` | 492 | 8551 | 8702 | 8702 | 0 |
| `spec-doc-r.md` | § 1's comment bullet: the doc shape, the example rule, `heroes doc`'s promise and refusal | `656b8364f33abaeb` | 498 | 8657 | 8814 | 8814 | +112 |
| `spec-skip-r.md` | § 12: `skip(reason:)` | `a47a36c1f157cd29` | 495 | 8609 | 8761 | 8761 | +59 |
| `spec-umb-r.md` | § 1: an entry's `packages`, and the example's `[dependencies.rebel]` | `07acf83d2ec062f9` | 500 | 8682 | 8836 | 8836 | +134 |
| `spec-stack-r.md` | § 6: *"saying why, where, and through which calls of the program it got there"* | `5f64ac703c5f7978` | 492 | 8564 | 8715 | 8715 | +13 |
| `spec-all-r.md` | doc-r, skip-r and umb-r | `48d807010b254507` | 509 | 8846 | 9007 | 9007 | +305 |
| `spec-all-r-stack.md` | and stack-r | `cb648afa1ec30da0` | 509 | 8859 | 9020 | 9020 | +318 |

design.md's arms (`design-r.md` +333, `design-c.md` +88, vendored) are under no ceiling (`./heroes-next
measure docs/design.md` says so); they are yours for Principle 0 and the record, not for the budget.

**The real counts are yours to take**: **at most seven refreshes**, one each of `now` (the pin re-measured
rather than carried), `doc-r`, `skip-r`, `umb-r`, `stack-r`, `all-r` and `all-r-stack`, counted at 0 USD
inside the sitting's 10 (`count_tokens` is free by Anthropic's page as panel 252's brief read it, carried,
`docs/panel/251-briefs/spec-warden.md:70-73`). `--refresh` refuses every path but `spec/heroes-spec.md`
and `CLAUDE.md`, so each arm is copied over your copy's `spec/heroes-spec.md`, refreshed, and reverted
(`cmp` against `bw/arms/spec-now.md` after each). The key: `set -a; .
/Users/joseph/Temp/heroes/heroes-lang/.env; set +a` in the one shell that refreshes, then `echo
${#ANTHROPIC_API_KEY}`, **its length and never its value**. Log each refresh in your report: the time, the
arm, the four constants printed. **Nothing you print is pasted into the tree.** Panel 251's spec-warden
refreshed in a copy with the `.git` file (carried, its report); if yours does not run, say so and the counts
stay vendored.

## What you are asked

1. **Q1, design.md §1.11.** Neither arm adds a spec token: confirm that the spec owes none (it names no
   library; § 13's first sentence, *"Anything beyond this document comes from packages or C libraries"*,
   line 377), or say what it owes and where. Judge the two arms as the record: does the conservative leave
   sentences of Part 1 a reader finds false (the table's *"PCRE2"* where Rebel's `regex` stands, *"That is
   the whole standard library"*), and is that a cost the conservative may carry? **Decision 10's
   register, from the set the milestone holds** (repaired after the critic's finding 11; the first list
   came from a word search): `bw/q1/core-milestone-issues.tsv`, every issue of milestone M-core-packages by
   `gh issue list --milestone M-core-packages --state all`, 231 issues, 19 decisions and 39 open (25
   defects, 10 features, 4 tasks), and `bw/q1/core-catalogues.txt`, the 17 programs of `examples/` whose
   catalogue names `core` 0.6.0 (`00-shared.md` § Question 1 lists them): propose for each decision
   *invalidated*, *stands* or *moves*, for each open issue its outcome (closed with a note, moved to
   M-rebel-library, kept), and say where the register lives (a file of `docs/`, the milestone page, a
   decision issue). CLAUDE.md:279 (*"A standard library"*
   among the places not to go) is the author's: say the words you would put to them.
2. **Q2, the doc comment.** (§ A7's *"check of the net"* is performed by no file, `00-shared.md` § Question 2: whatever the spec says, a check is to write.) Principle 0: the compiler does not need the shape; does it serve the thesis,
   measured by the blind seat's task t2 (asked only to show what the function answers, each parameter
   and an example, a reader of `now` writes its default, and a reader of `doc-r` the shape or not)? **Home**: § 1's comment bullet, where *"A comment
   directly above a declaration documents it"* already stands (spec lines 52-53), against § 12, where
   `test` blocks are; *"Merging beats appending"* (`.claude/rules/spec-shape.md` § Where a rule lives).
   **What the arm promises of a tool** (*"`heroes doc` prints ... the same bytes every time, and refuses a
   parameter line naming no parameter"*): does the spec state a verb's behaviour anywhere else (`heroes
   add json writes that entry`, line 35), and is a refusal by `heroes doc` a diagnostic class the spec must
   name (`.claude/rules/diagnostics-and-goldens.md`)? Panel 136's ruling (design.md:2601-2604: *"a documented
   test block extends it"*): does the arm say it, or does it need its own words?
3. **Q4, the skipped outcome.** Principle 0: the compiler's own tests have no service to wait for; does
   the form serve the thesis, by the 106 tests that read `ok` today without running their subject
   (`00-shared.md` § Question 4) and by task t4 (a reader of `now`, told only that the test *"cannot check
   the connection"* where no server is named, writes a silent pass or not: the count the hazard rule
   reads)? **The
   name**: a built-in's name is taken everywhere, a rule **the spec states nowhere** (task 2551, `OPEN`,
   *"The specification says that a built-in's name is taken everywhere, for a declaration, a parameter, a
   ..."*; `grep -n -i -E 'reserved|taken|shadow'` over the spec prints line 167's `use` rule and line
   442's *"is taken to keep"*, another sense): does `skip` owe that sentence first, and does the arm owe its renames (7
   declarations, `00-shared.md`)? **Where `skip` stands**: in § 12 beside `assert`, or in § 11's
   *"Built-ins:"* sentence, which the `offered` check holds to end at `range`
   (`tests/harness/spec_text.hero`)? And whether a run of nothing but skips is green, a sentence or not.
4. **Q5, § 6's sentence.** *"An abort ends the program at once, saying why"* (line 183) promises no place,
   and today's runtime says the place: is the place (and a stack) the spec's to promise, or the runtime's
   words a reader never reads from the spec? `spec-stack-r.md` (+13) or nothing; if the stack is
   promised, does Windows (*"no walk"*, `runtime/parts/stack.c:858`) make the sentence false there today?
5. **Q6, the umbrella.** `spec-umb-r.md` amends § 1 (lines 30-40) and its example: does *"places only
   those its `packages` names, with each one they use"* keep *"a program holds one version of each
   package"* (line 39-40) true, and the falsifier (`.claude/rules/cli-surface.md`: deleting or mistyping a
   field *"never changes which module compiles, only whether the program builds"*)? Is `packages` a field
   the catalogue may hold, or a fifth input class by another name (`cli-surface.md`, *"A fifth input class
   is refused"*)? Panel 250's spec-warden drafted p0, p1 and p1r for placement
   (`docs/panel/250-*.md:61-67`, carried): which of them, if any, the umbrella's route revives, and the
   conservative's 0 tokens with W. **And what the arm does not say** (the critic's finding 14): whether
   `packages` binds a `path` entry (spec line 34, *"by a local `path` alone, which nothing verifies"*);
   `examples-rebel/heroes.toml` is one `path` entry for 161 programs whose `use <rebel>/` lines name 163
   distinct paths. Either the arm owes words for it, or say why not.
6. **The payment** (design.md §1.6 from line 324): each addition owes a named removal or a registered
   prediction naming an instrument that exists the day it is registered. **Register yours first** (§ FIRST
   above), in terms the blind seat's counted rules score; say whether `spec-all-r.md`'s +305 vendored
   (and the real count you take) fits the consolidation block's 1,500 real tokens (line 292) beside what
   sittings A and B of this milestone spend (panel 251's R2 sentence; sitting B's briefs are
   `docs/panel/252-briefs/` if they exist when you read: say what you found).
7. **The diagnostic classes each route adds** (`.claude/rules/diagnostics-and-goldens.md` in your copy):
   Q2's three refusals of `heroes doc`, Q4's `skip` outside a `test` block and its taken name, Q6's
   unlisted package with its `certain` fix. What each owes: its annotation in the source that provokes it,
   its golden, its spec sentence or none.

## What you are asked to say

Your definition's structure per question; the measured deltas, real where refreshed and vendored where not,
each with its command; the sentence you would adopt word for word with its home; the predictions you
registered first and the counted rule that scores each; every premise of these briefs you found false.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
