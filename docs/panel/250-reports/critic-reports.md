# Panel 250, the completeness critic's second pass, over the reports

*Written as it went, 2026-11-15, from 08:50 (`date`, load averages 130.58 45.39 95.63) to 09:07.
Base `48f53b3437c6cd46aba5923f4f4780ac7b347283`; the worktree `.claude/worktrees/p250` printed
`48f53b343` for `git log --oneline -1` at 08:50, so the coordinator's correction (2) holds. Every
command below was run by the critic, read-only in the repository, in the seats' folders and in the
blind seat's folders, writing only under `.claude/worktrees/p250-scratch/critic/p2/` (below, `C/`) and
this report. No verdict on any route. No paid run, no API call, no `claude -p`, no `measure
--refresh`, no GitHub request of any kind, no build of a compiler (the two compilers used are byte
copies of the grading setup's, `C/ergo/grade/w/heroes` `6dbc55c8d1ce611d`, the base's stage 2,
`sources 5bac791363384945`, and `C/ergo/grade/m/heroes` `7fb56c9a07b8f1ab`, the compiler-engineer's
prototype `ce-m2` v2, `sources 1593fa56260f5ec3`, both printed by `--version`). One page fetched with
`curl` into `C/pages/`, inside the root. No process left (`pgrep -fl p250-scratch/critic` empty at
09:02 and at the end).*

## Read

Whole: `250-briefs/` (the eight current files; the seven `.before-critic` copies not read, since my
first pass reviewed them) and `250-reports/` (the five reports and my first pass). The coordinator's dated note at the end of
`250-briefs/critic-reports.md` is followed below: its (1) is § C, its (3) is § A and § B.

## Said first

1. **The coordinator's note (3) misstates two of the four voting seats** (§ A). Two seats object to
   placing only the reached modules and approve W (compiler-engineer, spec-warden). The ffi-pragmatist
   vetoes a computed C closure and **approves H** for a package's C, on two conditions, one of which is
   unbuilt and is broken by the prototype every later run used (measured here). The historian
   (advisory) **approves the reached grain by Go package**. Neither of those two gives a verdict on W.
2. **No unit of R2 lets feature 2315 meet R2 at `core` 0.5.0, and neither does R2 counted by reach
   (rM2)**: the unit decides only under a ruler nobody wrote down, the compiler's own modules counted
   with placed files left out (§ B). So the question the synthesis puts to the author is R2's *form*,
   not its unit, and panel 242's R7 already rules what happens if nothing changes: *"Feature 2315 waits
   for that package"* (`242-*.md:193-194`), a sentence no report and no brief quotes.
3. **The blind seat's grading followed the registered rule, with one reading made after the sessions
   that decides two of its three verdicts** (§ C): read strictly, rule 1 finds no command in 16 of the
   24 answers; Q1 holds either way, Q2 and the spec-warden's registered prediction hold only under the
   grader's reading. And its tasks told one history only; in the spec's own order (`heroes add` before
   the `use` line) p1r is false of the prototype it graded, measured here.

## A. What each seat's verdict is, read from its own lines

- compiler-engineer, `250-reports/compiler-engineer.md:17-20`: *"**object** to M, G and H ... **No
  veto** ... **Approve W**, placement unchanged."*
- spec-warden, `250-reports/spec-warden.md:21-24`: *"**object, provisional**, to M, G and H ... **veto,
  provisional, to p2** (F2) ... **approve, provisional, p0 under W**, with **rU** ... or **rM2**"*.
- ffi-pragmatist, `250-reports/ffi-pragmatist.md:18-22`: *"**veto** on M as the rule for a package's C
  ... **Approve** H's rule for C under the two conditions below, whatever grain the `.hero` files take
  (M or G). The veto does not reach placing only the reached `.hero` modules."*; and `:50-55`, condition
  (2), the residue (*"a C file of the package naming one of its own `.hero` files"*), closed by a rule
  it proposes **unbuilt**, *"Without (2) my verdict on H is **object**."* It says nothing of W but that
  a note's half *"stays true"* under W and H (`:85-88`).
- historian, `250-reports/historian.md:73-74`: *"**approve** (advisory; no veto), **of a program
  placing the reached grain, by Go package**"*.

**Condition (2) against the built prototype, run here** (`C/ask/ask.txt`): the ffi-pragmatist's `ask`
package copied from its index (`250-ffi-pragmatist/probes/index/ask`, read-only, copied to
`C/ask/index/`), its `main-ask.hero`, `heroes add ask --index file://../index` then `heroes build
main.hero -o ask-<w|m>` and the binary, each compile under `timeout -s KILL 300`, the binary under
`timeout -s KILL 60` through `head -c 65536`:

| compiler | `add` | placed | `build` | binary |
|---|---|---|---|---|
| base (`w`) | exit 0 | `a.h a.hero asked_only.h b.hero table.inc` | exit 0 | exit 0, `2 77 2` |
| prototype v2 (`m`) | exit 0 | the same less `b.hero`, and `ask@1.0.0-22be316d8a89e092.list` beside the folder | exit 0 | exit 0, `2 77 1` |

So the prototype the falsifier runs, the 18 re-placed programs, the differential probes and all 24
blind sessions' M cells were graded on is **H without condition (2)**, which the ffi-pragmatist's own
words make *object*. No seat's approval covers the prototype as built; the historian's approval is of
a grain, not of a build.

## B. R2's ruler: what would make it the question for the author, measured

**The compiler-engineer's counts reproduced by another instrument.** `C/series/tree_units.py` (my own
mirror of `tests/harness/suite_layout.hero:1034` `code_lines`, read before writing it: a non-blank line
outside a `test "` block, comments counted; placed trees left out of the walk) over copies of the
seat's trees (`cmpA1/`, `cmpC/`, `routeM/selfhost`, copied into `C/ce-copy/`) and a `git archive` of the
base's `selfhost/` (`C/series/tree_units.txt`):

| tree | lines | bytes | code lines | files |
|---|---|---|---|---|
| base `selfhost/` | 221,820 | 10,109,788 | 150,532 | 849 |
| route (A)'s modules | 221,853 | 10,104,662 | 150,486 | 849 |
| route (C)'s importers | 221,686 | 10,103,305 | 150,486 | 849 |
| route M v2's `selfhost/` | 222,209 | 10,126,028 | 150,826 | 851 |

Every figure equals the report's (`compiler-engineer.md:129-135`, `:321-322`). **Every version of
`core`, at the commit the brief names for it** (`C/series/series.py`, `git show` of each file, the
same `code_lines`; `C/series/series.txt`):

| version | whole: lines | code lines | bytes | files | the two reached `strings` files: lines / code lines / bytes |
|---|---|---|---|---|---|
| 0.1.0 | 522 | 215 | 21,039 | 6 | 368 / 166 / 14,034 |
| 0.2.0 | 2,389 | 1,369 | 93,113 | 13 | 368 / 166 / 14,034 |
| 0.2.1 | 4,259 | 3,030 | 153,194 | 15 | 368 / 166 / 14,034 |
| 0.3.0 | 5,962 | 3,921 | 226,634 | 26 | 368 / 166 / 14,034 |
| 0.4.0 | 6,140 | 3,993 | 235,363 | 26 | 368 / 166 / 14,034 |
| 0.5.0 | 6,574 | 4,211 | 253,311 | 30 | 368 / 166 / 14,034 |

`strings.hero` and `strings/internal/scan.hero` are **byte-identical at all six versions** (`git show
<c>:packages/core/<v>/files/<f> | shasum -a 256`: `cac66ea67a60021e` and `3a1b953c9bcee1da` at each).

**Each ruler at 0.5.0**, from the counts above (route (A)'s deletions, the only ones measured at this
base: −46 code lines, +33 lines, −5,126 bytes, no file; the catalogue 5 lines, 148 bytes):

| ruler | counts | lines | code lines | bytes | files | smaller? |
|---|---|---|---|---|---|---|
| R2 as written (and rU), W | modules, 30 placed files, catalogue | +6,612 | +4,165 | +248,333 | +31 | no, any unit |
| R2 as written, M strict | modules, 2 placed, catalogue | +406 | +120 | +9,056 | +3 | no, any unit |
| **rM2**, W | modules, the 2 reached, catalogue | +406 | +120 | +9,056 | +3 | **no, any unit** |
| **rS** (not listed): the compiler's own modules, placed files not counted | modules, catalogue | +38 | **−46** | **−4,978** | +1 | **code lines and bytes yes; lines and files no** |
| the same without the catalogue | modules | +33 | −46 | −5,126 | 0 | code lines and bytes yes; lines no; files equal |

**What this settles.** The unit decides nothing under R2 as written or under rM2: every unit grows
(the compiler-engineer's table, `:262-271`, reproduced). It decides only under rS, which is route 9's
count without route 9's `path` entry, and which is what the spec-warden reads *"placed files counted"*
as written to forbid (`spec-warden.md:373-375`). So **the question for the author is R2's form**, and
its default is already ratified: panel 242's R7, *"Then the compiler's first package by R2's ruler,
measured when one exists ... Feature 2315 waits for that package"* (`242-*.md:192-194`), which is the
author's *c* of 2026-11-14 (`M-core-packages.md:506-507`, *"b was moving 2315 to a later milestone, c
waiting"*). `grep -n -i 'waits for that package\|2315 waits'` over the five reports and the briefs:
nothing.

**What the wait costs under each form, arithmetic over the measured parts** (this base's deletions
held fixed, which panel 242 measured otherwise at `1edabb957`): under **W with R2 as written**, the
step's placed code lines were 215, 1,369, 3,030, 3,921, 3,993, 4,211 from 0.1.0 to 0.5.0, so the gap
was +169, +1,323, +2,984, +3,875, +3,947, +4,165: it has grown at every version, and closes only for a
version whose whole tree is at most 45 code lines, or after about 4,166 more code lines of copies are
deleted. Under **rM2** the gap is +120 code lines (+406 lines) at every version, since the reached
files have not changed; it closes after 121 more code lines of `strings`' contract are deleted, or a
smaller `strings`. Under **rS** it is closed today in code lines and bytes.

**How many code lines a deletion could find, by vocabulary** (`C/series/nearcopies.py`, each top-level
function whose line matches the shape, its comment block and body, non-blank lines, outside `## Tests`;
`C/series/near-split.txt`, `near-trim.txt`): `(…: str) -> [str]` functions named line, word, field or
split, **27, 394 code lines**; `(…: str) -> str` named trim, strip, space or blank, **22, 163 code
lines**. An upper bound by names: the compiler-engineer read 30 and 22 of them and found none a copy
of `core`'s by behaviour (`compiler-engineer.md:138-146`). Turning some into copies (each adopting
`core`'s treatment of `\r` or Unicode white space) is a route nobody listed: it changes what the
compiler does, so it is rank 3's question before it is R2's, and its instrument is the differential run
of each near-copy on its callers' inputs that route 2 also needs and nobody ran.

**And what 2315 would deliver if a ruler admitted it.** The compiler-engineer counts *"0 of the 10 C
groups and 1 helper"* replaceable by 0.5.0 (`:363-378`). Re-run: `grep -n '^extern "'
selfhost/fetch/*.hero selfhost/module/*.hero | wc -l` in `C/ce-copy/base`: **10**, but in **6
modules** (`fingerprint`, `reading`, `spelling`, `index_source`, `leftover`, `run_lock`), not the
report's 7. So under rS or any admitting form, *"the package manager rewritten over the core packages"*
is route (A)'s 52 call moves in `fetch/` and `module/` with no function deleted there (the report's
count, not re-run), which is a fact the author's choice turns on and no brief asked.

## C. The blind seat: did the grading follow the registered rule

**Held, each run here.** The key and the folders: `key.txt`'s three hashes against each of the 24
folders' `brief.md`, `task.md`, `heroes-spec.md`, **24 of 24 equal**, each folder holding those three
and `run.json`, `run.err`; arm d's spec is `git show 48f53b343:spec/heroes-spec.md` (`7dc8bdaa…`, the
same hash), and `diff arms/spec-d.md arms/spec-h.md` prints line 35 alone. `run.sh` hashes
`5f8229ec34d39497`, the registered hash, and its `env -i` line carries `GIT_CEILING_DIRECTORIES` (line
58; the key variable masked in what I printed). The instruments hash as registered (`w/heroes`
`6dbc55c8d1ce611d`, `m/heroes` `7fb56c9a07b8f1ab`, `run-answer.sh` `8145d0e2193cdd92`,
`t1-ref.hero` `33f840132d807a4b`, `t3-ref.hero` `183662d587130d78`), the two runtimes equal by `diff
-rq`. `grade/answers/<f>.md` byte-equal to `run.json`'s `result`, **24 of 24** (`C/ergo/extract.py`).
`grade.log` holds **80** runs (`grep -c '^### '`). **Five cells re-run** with my copy of the setup
(`C/ergo/grade/regrade.txt`): `t1-d-1` W and M, `t2-h-1` W and M, `t3-d-2` M, every line's exit, code
and output equal to `grade.log`'s (the build keys differ, the folders' paths being in them).

**The reading that decides.** Rule 1 (`250-briefs/llm-ergonomist.md:222-225`): commands are the lines
that *"stand in a code block or as an item of a numbered or bulleted list of steps and begin, once
leading white space and a `$ ` or `% ` prompt are taken off, with `heroes `"*. Every t1 and t3 answer
writes its steps as ``1. `heroes build main.hero` ``. `C/ergo/extract.py`, both readings
(`C/ergo/extract.txt`): **strictly, 16 of 24 answers hold no command** (every t1 and t3), the eight t2
answers the same commands as the grader's; with a list item's inline code read as its text, the
grader's reading 1 (`llm-ergonomist.md:56-57`), every answer's commands are the grader's. Under the
strict reading, read in M: arm h C-right 4 (t2 only), arm d 0, so **Q1's difference is 4, still the
registered 4: p1r enters with M either way**; **Q2 fails** (arm h 4 of 8 t1 and t2 sessions, 6 asked; 0
of 4 t3, 3 asked); **the spec-warden's prediction fails on arm h** (4 of 8, 6 asked). My reading of
rule 1: a list item's marker must be taken off under every reading, and the backticks are markup of the
same kind, so the grader's is the reading under which the list clause can match a Markdown list at
all; it was written down before the grades. But it was not registered, and the synthesis should say
that Q2's *"p1r suffices as worded"* and the prediction's *"holds on both halves"* rest on it, Q1 not.

**The history the tasks did not tell.** Every task tells the `use` line written *before* `heroes add`,
by design (`250-briefs/llm-ergonomist.md:206-210`), because the prototype's `add` with no `use` naming
the entry places the version whole. Run here (`C/ergo/grade/addfirst.txt`): a `main.hero` printing
`3` with no `use`, `heroes add text --index file://../../index` under each compiler, exit 0 and **30
files placed** under both, no listing; then the t1/t2 edited program written and `heroes build
main.hero`: **exit 0, `3/12/` under the prototype**, no fetch; a `heroes fetch` after it exit 0, still
30 files. So in the spec's own order p1r's *"a `use` written after it is refused until it runs again"*
predicts a refusal the graded M does not make, the shape of arm h's W cell (8 of 12, the four t2
answers *invented*). Q2's verdict covers the use-before-add history alone; the add-first cell is
unmeasured for readers (a paid run) and measured here for the tool.

**Recorded, not decided.** 18 of 24 context sections name sibling folders (`grep -E
't[123]-[dh]-[1-4]'` over each `## context`: the grader's 18, the same names); `t2-h-1` writes *"The
user's message, which lists other task identifiers (t1-d-1 … t3-h-4) as names only"*, while
`run.sh`'s prompt names none (its line 59). Where the list came from is untraced, a question for the
coordinator; no session answered `yes`, so the registered rule voids none. **And Q1's result is
conditional**: *"p1r enters with M"*, where no voting seat recommends M; under W, p1r is false (arm h in
W, 8 of 12).

## D. Contradictions between seats, and which side the run takes

1. **The verbs' help.** The spec-warden: *"The verbs' help is not made false by M"* (`:185-189`). The
   prototype changes it: `diff` of `cli/table.hero` (`C/ce-copy`), `add`'s *"and places each"* becomes
   *"and places what the program reaches of each"*, `fetch`'s *"place every package heroes.toml names"*
   becomes *"place what the program's use lines reach of every package heroes.toml names"*. If M lands,
   the help a reader reads moves (the compiler-engineer's *"Said first"* says so, `:8-10`).
2. **`unknown_entry`'s *"a package is fetched whole"***, which the spec-warden reads false under M
   (`:192-195`), stands unchanged in the prototype (`grep -rn -F 'a package is fetched whole'`:
   `package_told.hero:54` in route M, `:53` at the base). The graded prototype carries it.
3. **H's count for the compiler.** The spec-warden: *"H counts 419"* (`:390`), one card. H as the brief
   defines it places every file that is not `.hero` (`00-shared.md:431-432`), and so the
   ffi-pragmatist (`:74-75`) and the prototype read it: 368 + 149 (7 cards) + 30 (listing) = **547**
   lines (`compiler-engineer.md:251-252`, 9 files 517 lines and the listing 30). No verdict moves.
4. **"No golden moves."** The compiler-engineer's differential probe, 44 and 11 cases the same
   (`:337-347`). Every one of the 55 holds a tree placed by hand with no listing, so the prototype read
   each whole; and `find 250-compiler-engineer/difffetch -name '*.list'` prints **0**: no golden case
   placed a subset. The probe shows that the whole-tree fall-back keeps today's answers; it measures
   nothing of what M does in a golden case. The spec-warden's question whether the *"places it again"*
   goldens move (`:200-205`) is answered for the fall-back only.
5. **The split and the `Use` production.** The spec-warden's reading that an entry name cannot hold a
   path (`:166-170`), left unrun by the compiler-engineer (`:412-413`). Run with the base compiler
   (`C/split/check.txt`): `use <encoding/json>/json`, `check` **exit 1**, *"error[package_mark_holds_a_path]:
   only the package's name goes between `<` `>`, its module after them — `use <encoding>/json`"*. The
   split as 00-shared.md names it (`encoding/json` an entry) cannot be spelled.
6. **The coordinator's note (3) against the reports**: § A.

## E. Precedents, and sentences about other tools

- **The historian's two wordings of Go's `go mod vendor` sentence** (§ 1, *"containing copies of all
  packages needed to build and test"*; its candidate line, *"that contains copies of all packages needed
  to support builds and tests"*) differ. `curl -s -L https://go.dev/ref/mod` into `C/pages/gomod.html`
  (HTTP 200, 283,851 bytes): **both stand on the page**, the first in § Vendoring, the second in § `go
  mod vendor`; so does *"The `go` command does not check that packages in the `vendor` directory have
  not been modified"*. Verified.
- **A sentence of the same page no seat connected**: *"build constraints except for `ignore` are not
  considered when constructing the `vendor` directory"* (on the page, both sections; quoted in the
  historian's § 1, `:101-102`): Go's copy carries every platform's files of a package. That is the
  platform half of the ffi-pragmatist's case for H (its `arms` measurements); the historian's reading for
  H cites only golang/go#26366, the directory half (`:519-522`).
- **The compiler-engineer's *"Python's `strip`, which also takes U+001C to U+001F off"*** (`:144-145`),
  a sentence about another language: `python3 -I -c` on `'\x1c\x1d\x1e\x1f a \x1f\x1e\x1d\x1c'.strip()`
  printed `'a'` (Python 3.15.2). Holds.
- **The historian's argument** writes *"Deno fetches only imported JSR modules"* (`:552-553`) without the
  hedge its own item 6 and § 3 carry (a post by the runtime's author; *"no sentence saying clients
  download only the modules they need"* on JSR's API page). The candidate line is right; the argument's
  sentence says more than its pages.
- **Where a fetched page landed** (the historian's § 12, *"unknown to this seat"*): `find` of this
  session's `tool-results/` and `subagents/` under `~/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/15c2275d-…/`
  for files newer than 2026-11-14 22:00, transcripts left out: 10 in `tool-results/`, each the
  coordinator's own long outputs by its first bytes (git logs, a rule file, a census, a milestone page),
  none a fetched page; none in `subagents/`. Read, nothing written there.

## F. Unrun, and owed before the synthesis adopts anything that depends on it

- **The net, under any route.** The guard refuses every compiler built in a copy with no `.git`,
  judging it against the trunk (`compiler-engineer.md:327-336`); so the `fetch` and `packages` suites, the
  55 golden cases, `emission`, `warnings`, `corpus`, `canonical` and the site's build are **unrun under the
  prototype by anyone**. 00-shared.md's *"a narrowed suite does"* run in a copy (`:113-115`) carried my
  first pass's run, which the rebase made false. Owed to a worktree with its own `.git` at the base.
- **A case that lists the files placed**: asked by the brief (`compiler-engineer.md` brief, step 4),
  *"Not built and owed"* (`compiler-engineer.md:36-38`).
- **The `packages` suite's `placed` check** holds *"every tree under a `heroes.packages/` is ... the
  index's files byte for byte"* (`tests/harness/suite_packages.hero:33-36`): an M-placed example fails it
  by its words. A reading, unrun.
- **A git source under M**: no run placed a subset from one (the 18 programs read the index; the 11
  fetch cases kept whole trees, 0 listings). The verb's computed listing is unexercised.
- **A closure across packages**: the prototype places whole an entry reached only through another
  package (`compiler-engineer.md:190-191`); the ffi-pragmatist's shape 5 was measured as published
  subsets, not through the prototype.
- **Condition (2)'s closure**: unbuilt; the residue measured present (§ A).
- **Route 2, R2's defect branch**: *"Read, not run"* (`compiler-engineer.md:288-290`).
- **rM2's instrument**: unbuilt (`compiler-engineer.md:402-407`); its count was made by hand.
- **The real token deltas** of p1r, fM, rM2, rU: vendored only, lower bounds (no `--refresh`).
- **Windows**: owed to the CI's leg (box off).

What is measured and holds: the emitted C does not move where the reached modules are the same. The
compiler-engineer's `ini` and `server`, and here a third program under another key (`C/ergo/emitcmp.txt`,
the blind seat's start program, key `text@0.5.0-554b61376b526680`): base compiler on the whole tree,
prototype on the whole tree, prototype on its 9-file part, each `--emit-c` exit 0, 46,499 bytes, 13
`#line`s naming the placed folder, `cmp` silent for both pairs, the `hero_program_sources` line in. And
the prototype's placed listing is the index's `list` byte for byte (`cmp`, for `ask` and for `text`), so
the spec-warden's first condition for *"no fifth class"* (`:311-313`) holds for an index source.

## G. The reader-facing escape

W, rU, rM2 and rS change nothing a reader of a program meets (design.md's ruler alone). Every report
that built or read M names it reader-facing in the brief's words; the synthesis escapes only by
adopting M, or p1r without the add-first history measured (§ C). Two reader-facing facts no seat put in
its *"said first"*: **under the prototype one catalogue and one set of `use` lines give two placed trees**
by the order the verbs ran (here 30 files after add-first against the start state's 9 and a listing,
`find heroes.packages -type f | wc -l`), and the site's build renders placed trees
(`site/src/lib/examples-model.ts:205-255`, carried from the brief). And three reader-facing defects
**under W, today**, which the synthesis must hand on whatever it adopts (the ffi-pragmatist's
`:78-88`, `:137-140`): `package_reads_outside` naming a file inside the root as outside on a fetched
header's `#embed` (class `blocking` by the rules, *"a false message"*, unfiled); `ffi_missing_header`'s
note inexact under W for a unit reaching another package's record; README § 4 rule 5 and panel 241's
R7 not enforced for a header with platform arms. And feature 2315's notes still name the withdrawn
`selfhost/core/` (the spec-warden's `:299-303`), a `comment` owed.

## H. What the sitting should have asked

1. **R2's form, put to the author with § B's table**: keep it (242's R7: 2315 waits, under W for a
   version that has never existed), rM2 (waits for 121 code lines), rS (met today in code lines and
   bytes, not in lines), or 2315 moved (*b*). Who: the coordinator, from the counts above and the
   compiler-engineer's; nothing more to run for the table, and route 2's differential run for the
   defect branch.
2. **What 2315 delivers at 0.5.0 under an admitting ruler**: 0 of 10 C groups, one helper, 52 calls
   moved. Who: the author, on the compiler-engineer's count.
3. **Whether near-copies may become copies** (§ B's 394 and 163 code lines by name): the
   compiler-engineer, by a differential run per function on its callers' inputs.
4. **Two places count a placed tree opposite ways**: R2 counts placed files for admission; the
   `layout` suite leaves them out of §11's ceiling, *"a tree a fetch places under `selfhost/` is no
   module of the compiler's to count"* (`tests/harness/suite_layout.hero:1067-1071`, citing panel 242's
   R7 and feature 2425). rS is that second reading applied to R2. The spec-warden reads which one
   design.md §1.11 means.
5. **If M is ever adopted**: the add-first cell read blind (a paid run), and the history-dependent
   placement ruled (prune on `fetch`, or `add` placing the reach), before p1r's wording is.

## The list

1. **The coordinator's note (3), *"Every seat objects to placing only the reached modules and approves
   whole placement (W)"*, is contradicted by two reports.** Read: `ffi-pragmatist.md:18-22` (*"Approve
   H's rule for C ... The veto does not reach placing only the reached `.hero` modules"*);
   `historian.md:73-74` (*"approve ... of a program placing the reached grain, by Go package"*); neither
   gives a verdict on W. **Must not adopt** a record saying every seat approves W: two of four do.
2. **The prototype every later run used fails the ffi-pragmatist's condition (2).** Run: its `ask`
   package, both builds and both binaries exit 0, the base's printing `2 77 2`, the prototype v2's
   `2 77 1`, `b.hero` left out of its placement (`C/ask/ask.txt`). By `ffi-pragmatist.md:54-55` its verdict on H as built is *object*. **Must not
   adopt** H, or any grade made on the prototype, as approved by the ffi-pragmatist without the closure
   it proposes, built and run.
3. **R2's unit is not the question; its form is.** Run: my `code_lines` mirror reproduces the
   compiler-engineer's units exactly (`C/series/tree_units.txt`); every version's counts
   (`C/series/series.txt`); `strings`' two files byte-identical 0.1.0 to 0.5.0. Under R2 as written and
   under rM2, every unit grows (+4,165 and +120 code lines at 0.5.0); only rS (own modules, placed files
   not counted, unlisted) shrinks, in code lines (−46) and bytes (−4,978 with the catalogue), not in
   lines (+38) or files (+1). **Must not adopt** rM2 as unblocking 2315, nor put *"which unit"* to the
   author as if it decided anything outside rS.
4. **Panel 242's R7 already rules the default**: *"Feature 2315 waits for that package"*
   (`242-*.md:193-194`). `grep` over the reports and briefs: not quoted. Under W the gap has grown at
   every version (+169 to +4,165 code lines, arithmetic over measured parts); under rM2 it is a constant
   +120. **Must not adopt** a resolution that leaves 2315's fate unstated: the question to the author is
   b, c (R7 as ratified), rM2, rS, or the defect branch, with § B's table.
5. **The blind seat's Q2 and the spec-warden's prediction rest on a reading made after the sessions.**
   Run: `C/ergo/extract.py`, strict rule 1 finds no command in 16 of 24 answers; strictly, Q1 holds (4),
   Q2 fails (4 of 8, 0 of 4), the prediction fails on arm h (4 of 8). Everything else held: hashes 24 of
   24, answers byte-equal 24 of 24, 80 runs, five cells re-run equal. **Must not adopt** *"p1r suffices
   as worded"* or *"the prediction holds"* without saying they rest on the grader's reading 1.
6. **p1r is false of the graded prototype in the spec's own order.** Run (`C/ergo/grade/addfirst.txt`):
   `add` before any `use` places 30 files, no listing; the edited program then builds at exit 0 printing
   `3/12/`, no fetch. The tasks told only use-before-add. **Must not adopt** p1r's wording for any M
   without the add-first history ruled and read.
7. **18 sessions saw sibling folders' names, from a source not traced**: `grep` over the context
   sections gives the same 18; `t2-h-1` names *"the user's message"* as the source, and `run.sh`'s prompt
   holds no list. Unrun: the session transcripts, which `run.json` does not keep. **Must not adopt** a
   claim that the folders held three files *"and nothing else"* reached the sessions, without saying so.
8. **The prototype changes the verbs' help and keeps a message the spec-warden reads false under M.**
   Run: `diff` of `cli/table.hero` (both summaries rewritten), `grep -rn -F 'a package is fetched
   whole'` (unchanged, `package_told.hero:54`). **Must not adopt** M with *"the help stays true"* or with
   that message as it stands.
9. **"No golden moves" exercised no subset placement.** Run: `find .../difffetch -name '*.list'`, 0;
   every one of the 55 hand-placed trees holds no listing, so the fall-back read each whole. **Must not
   adopt** it as a measurement of what M does to a golden case.
10. **The net is unrun under any route**: the guard refuses a copy's compiler
    (`compiler-engineer.md:327-336`); `fetch`, `packages`, the 55 goldens, `emission`, `warnings`,
    `corpus`, the site's build are owed to a worktree with its own `.git`; the `packages` suite's `placed`
    check fails M-placed examples by its own words (`suite_packages.hero:33-36`, read, unrun); no case
    lists the files placed. **Must not adopt** M on a probe that stands in for the net.
11. **The split as named cannot be spelled.** Run: `use <encoding/json>/json`, `check` exit 1,
    `error[package_mark_holds_a_path]` (`C/split/check.txt`). **Must not adopt** a split with
    `encoding/json` or `net/http` as entry names.
12. **A count to correct**: the package manager's 10 `extern` groups stand in 6 modules, not 7 (`grep -c
    '^extern "' selfhost/fetch/*.hero selfhost/module/*.hero`, the non-zero lines). And the
    spec-warden's *"H counts 419"* is 547 by H's definition. No verdict moves; the synthesis copies the
    corrected figures.
13. **Precedents checked**: both of the historian's Go wordings are on go.dev/ref/mod (`curl`, HTTP 200);
    the compiler-engineer's Python sentence holds (`python3 -I`, `'a'`); the historian's argument line
    on Deno says more than its pages (`historian.md:553` against its item 6). Go's *"build constraints
    except for `ignore` are not considered"*, on the page, is the platform precedent for H that no seat
    cited. **Must not adopt** the Deno sentence as a precedent's words.
14. **Three reader-facing defects under W today**, found by the ffi-pragmatist and unfiled
    (`ffi-pragmatist.md:78-88`, `:137-140`), and 2315's stale `selfhost/core/` note. **Must not drop**
    them whatever the resolution; they are the coordinator's to file and comment.
15. **Unbuilt, and named as such**: the residue's closure, a closure across packages, a git source under
    M, rM2's instrument, route 2's differential run, the near-copies' differential run, the real token
    deltas, the Windows leg. **Must not adopt** a route that leans on any of them as if it were run.
