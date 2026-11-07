# Panel 235, shared brief: M-package-manager's opening sitting

**Convened 2026-11-07** in M-package-manager (row 77), as its opening sitting,
by the author's instruction of 2026-11-06 (*call a sitting for these
choices*), which is the once-per-milestone yes of CLAUDE.md § 4. Five seats and
a completeness critic; **the critic reads these briefs before any seat starts**
(`docs/panel/235-reports/critic-briefs.md`), and the briefs are repaired on it.
`proposal.md` is the proposal in ten lines. `probe/README.md` holds every fact
about the tree this brief states, each with its command and its whole output.

## Read first

1. `docs/roadmap/milestones/M-package-manager.md`, from the paragraph that
   opens *Opened 2026-11-07* to the end: the author's position in seven points,
   the criteria, the author's answers, the order in three tempos, the
   sitting's questions, the author's word on CLAUDE.md § 2, and the author's
   position after the survey.
2. `docs/panel/235-reports/historian-survey.md`, the survey of thirteen package
   systems and seven indexes the author read before this sitting (18,190
   words, every cell sourced, nothing run).
3. `probe/README.md`.

## Frames no seat argues past

1. **The author's rulings are positions, not proposals** (the page): a
   dependency is named in code by an alias between angle brackets that the
   program's catalogue resolves, `use <json>/parse`, and the domain never
   appears in the code (position 3); a `use` with no brackets stays a module
   of the program's own tree, and *the bracket is the whole difference between
   mine and fetched* (position 7); one base, Go's modules or Cargo, copied
   whole in its newest release, without workspaces, with Heroes' own names; an
   index of Heroes packages from the first day beside git; the catalogue is
   optional, so a program without one compiles as today; three tempos, the
   first version over C. The sitting rules **how**, and rules between Go and
   Cargo and on the catalogue's format, which the author left to it (the
   author's word after the survey, 2026-11-07). **The spelling is not the
   sitting's to rule**: it rules the spelling's grammar, its mistakes and their
   messages. The reader test runs two controls beside the author's form
   (U-bare and U-pkg, `blind/README.md`), because a measurement outranks an
   opinion, the author's included (CLAUDE.md § 12); a control that beats the
   author's form by the README's registered rule is **a measured case put to
   the author**, never a resolution the sitting adopts.
2. **The author's criterion is measured, never assumed**: *the most modern and
   current solution, the one that works best for language models*, beside
   *the most solid, robust and simple, with small conventions accepted*. The
   blind seat's reader test (`blind/README.md`) is its instrument; a seat's
   claim about what models write is a question unless that test measured it.
3. **Robustness ranks above elegance, cost and speed** (CLAUDE.md §
   Precedence, rank 3): a Heroes program must not segfault and must not
   corrupt memory, and a fetched dependency is code the program trusts.
4. **design.md §1.11 stands**: no standard library; a package is code a
   program carries (panel 234's R1, ratified 2026-11-06).
5. **CLAUDE.md § 2** forbids *no declared version, no mode, no old spelling
   kept beside the new* before `1.0.0` (probe); **the author's word of
   2026-11-07** relaxes it *if it is needed*: a line declaring a language
   version or an edition may stand where this sitting measures the need.
6. **No paid run but the ones named here**, together within the author's cap
   of **25 USD for the whole sitting**: the reader test (hard stop 19.00 USD),
   the blind seat's one session (`llm-ergonomist.md`, at most 3.00 USD), one
   probe of the output cap (at most 0.05 USD), all three charged to one ledger
   that the runner stops at 22.00 USD overall, reserving for every running
   session its cap plus one capped call; and the spec-warden's `heroes measure
   --refresh`, within the 3.00 USD left, its calls counted in its report.

## The tree

Every seat works in its own worktree, `.claude/worktrees/235-<seat>/`,
detached at `235-base`: one merge commit, never pushed, whose first parent is
the lane `pm-opening` at the commit holding these briefs as repaired on the
critic's first pass, and whose second is `core-b1` at `9791d4787`, the first
batch of the core packages, so the tree holds the 14 programs that carry
`core/`. `git log -1 --format='%H %P'` in your worktree names both, and
`git rev-parse HEAD^{tree}` the tree, which the synthesis records because
the commit dies with its worktree. The probe was run at `9d894fe68` (parents
`6656268fe` and `9791d4787`, tree `f01b6059aeb1f0f2d86c085fecbdb7eb0a688f43`),
an earlier merge of the same two lanes before the briefs. **Build your own compiler
from the seed** (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
then `./heroes build selfhost/main.hero -o heroes-next` where a seat changes
`selfhost/`); never use another seat's or the trunk's. Your scratch is
`.claude/worktrees/235-<seat>-scratch/`. **No file is ever written outside
`/Users/joseph/Temp/heroes/heroes-lang`.**

## What the tree says today (probe)

- `use` names one file from the directory of the compiled file:
  `Use = "use" ident { "/" ident } [ "as" ident ] NEWLINE`, and a dependency's
  own `use` lines would be read from the consumer's root
  (`selfhost/modules.hero:107`).
- `use <json>/parse` is refused at 1:1, the caret under `use`, by
  `expected_module_name`, whose text names the `<` it found;
  `use pkg/json/parse` parses and is refused only because no folder holds it
  (`unknown_module`); `use github.com/acme/json` is refused at the dot
  (`module_path_has_no_parts`).
- The compiler runs other programs (`selfhost/cli/process.hero:109`, `run`);
  its one network access is `heroes measure --refresh`, which runs `curl`
  (`selfhost/cli/refresh.hero:4`); SHA-256 is written in Heroes over a `str`
  (`selfhost/cli/sha256.hero:119`, 292 lines), so a file that is not UTF-8
  has no fingerprint today.
- The command has 13 verbs, pinned by `assert table.len() == 13`
  (`selfhost/cli/table.hero:238`); the site counts them.
- A per-project file is refused by `.claude/rules/cli-surface.md` § There is
  no fourth slot, conditional on panel 056's three return conditions, *its
  only amendment path*; the second, *no string in the file may also appear in
  a `.hero` file*, cannot hold for a catalogue whose entries the `use` lines
  name. Its purpose, in design.md §3.5's words, is that *deleting the file
  never changes which symbols are linked*; question 17 below asks it of a
  catalogue. The rule file cites `heroes add` at `design.md:772`, a stale
  line: it stands at `docs/design.md:898`, in §3.5 (`grep -n 'heroes add'
  docs/design.md`, 2026-11-07).
- The 14 copies of `core/` in `235-base` carry 19 modules under an
  `internal/` folder (`find examples -path '*internal*' -name '*.hero' |
  wc -l`), for example `examples/ini/core/strings/internal/scan.hero`.
- 5,482 `use` lines in `selfhost/`, 635 in `tests/harness/`, 130 in
  `examples/`, 41 of them `use core/…`; 69 programs in `v0.6.0`'s
  `examples/`, which `compat` builds.
- `records` exempts `vendor` only at the repository's root
  (`tests/harness/suite_records.hero:84`).

## The sitting's questions

0. **The base**: Go's modules or Cargo, one and whole, and where the author's
   rules bend it (the survey's Table 1 is the inventory of both).
1. **The words**: a file is a module (spec § 1); the names of a folder of
   modules, the unit with a version, the catalogue, the fingerprint file, the
   verbs.
2. **The spelling's grammar and mistakes**: the author's `use <name>/…`
   (Frame 1) as a production of spec § 1, and every plausible mistake (the
   brackets forgotten or added, the whole path inside them as C's `#include
   <a/b>` writes it, the domain as Go writes it, `import`) as an error with a
   `certain` fix; the two controls are measured, not ruled.
3. **Inside a dependency**, its own `use` lines start from its own root.
4. **The catalogue's file and format** (YAML, TOML, JSON, a line format,
   Heroes' own syntax), and whether reading it can run code.
5. **An entry**: its sources (the index, git, a local folder), its exact
   version, its fingerprint, over which bytes, and how a file that is not
   text is read, copied and fingerprinted.
6. **Versions**: exact pins or ranges, minimal version selection or a solver,
   one version per program.
7. **Where fetched code lives** (inside the program, a cache, both), and what
   `corpus`, `packages`, `records` and `unseen` must know of it.
8. **The two sources and the index**: the index's protocol, a static index as
   one variant, names on the index, a withdrawn version.
9. **The catalogue is optional.**
10. **The three-inputs rule rewritten, not worked around**: the point is the
    author's call, recorded `retro-record`; `cli-surface.md` and design.md
    §3.5 amended in one commit, with a falsifier.
11. **Panel 234's R2**: `core` as a catalogue entry, its canonical tree, and
    what the `packages` suite becomes.
12. **`internal/`, the verbs**, and the site's count of them.
13. **The C under the first version**: programs run (`git`, `curl`) or
    libraries bound, and what each does to M-standalone-compiler (row 83).
14. **The interface that stays fixed** from the first version to the second,
    and what the second replaces with which core packages.
15. **What row 78, M-package-index, must deliver**, and what of it the first
    version must already respect.
16. **Alias scope**: whose catalogue resolves a dependency's own `use <x>/…`
    lines; what happens when two catalogues give one alias to two packages, or
    two aliases to one, under one version per program; and whether an index
    package may be named otherwise than its index name, as a git entry may
    (the reader test states that it may not, a simplification of the test and
    not a ruling).
17. **Panel 056's second condition by its purpose**: deleting or mistyping a
    catalogue entry never changes which module compiles, only whether the
    program builds. Which spellings and placements of fetched code meet it.
18. **The network**: may `build`, `check` or `run` ever fetch, or only the
    verb that adds or fetches; `selfhost/cli/refresh.hero:4-10` admits the
    network *only where the answer is pre-recorded*.
19. **Locality**: `use <json>/parse` means what another file, the catalogue,
    says. The blind seat (`llm-ergonomist.md`) holds the veto on non-local
    constructs and judges it, its verdict row in the synthesis.
20. **The simplicity the author asked to be measured**: the new words and
    concepts a reader must learn, counted; the catalogue's grammar in
    productions; the files a program must have, zero when it depends on
    nothing.
21. **Question 0 under the author's criterion**: how *the one that works best
    for language models* is judged between Go and Cargo, which the reader
    test does not vary; the test's habit count (which ecosystem's habits the
    models' unprompted mistakes show) is one instrument.

**Who takes which.** Every seat may write on any question; these are owed.

| seat | owes |
|---|---|
| compiler-engineer | 0, 2, 3, 4 (each parser in lines and productions), 5 (the fingerprint's bytes), 6, 7, 9 (the 69 programs `compat` builds), 11, 12, 14, 16, 20 (productions) |
| ffi-pragmatist | 5 (over which bytes, a file not text, whose fingerprint is trusted), 8, 13, 15 (with the historian), 18, robustness at the boundary |
| spec-warden | the predictions first, every variant's tokens, Principle 0 for each new form, § 2, 10, 17, 20 (words, concepts, files), 21 (with the historian) |
| historian | 0, 1, every precedent the resolution rests on, 8, 15, 21 |
| llm-ergonomist, blind | 2 and 19 from the specification alone, with the veto on non-local constructs |
| critic | its two passes |

## How a seat works and reports

- Your report is the lane's file
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/pm-opening/docs/panel/235-reports/<seat>.md`,
  written **as you go**, one section per question you take.
- **Every number is a command you ran, named beside it; a negative sentence is
  a command you ran, or it is written as a question.** A fact this brief
  states that you find false is the first thing your report says.
- Close with the verdict table's row: verdict (approve, object, veto where
  your seat has one), the sections of design.md it rests on, the cost or
  delta, a prediction with the milestone that can score it, and the condition
  that would change your verdict.
- Run no paid model session and call no API unless your brief names it; print
  no credential, ever (`gh auth status` masks a token, `gh auth token` is
  refused); a program that may not end runs as a built binary under `timeout`,
  its output bounded. Amphetamine holds the machine awake (`pmset -g
  assertions` read 2026-11-07).
- Write nothing outside your report, your worktree and your scratch.
