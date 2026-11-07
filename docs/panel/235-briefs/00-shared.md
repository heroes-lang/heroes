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
   dependency is named in code by an alias the program's catalogue resolves,
   and the domain never appears in the code; a `use` with no mark stays a
   module of the program's own tree; one base, Go's modules or Cargo, copied
   whole in its newest release, without workspaces, with Heroes' own names; an
   index of Heroes packages from the first day beside git; the catalogue is
   optional, so a program without one compiles as today; three tempos, the
   first version over C. The sitting rules **how**, and rules between Go and
   Cargo and on the catalogue's format, which the author left to it.
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
6. **No paid run but the ones named here**: the blind seat's reader test and
   the spec-warden's `heroes measure --refresh`, together within the author's
   cap of **25 USD for the whole sitting**, the reader test hard-stopped at
   22.00 USD by its runner.

## The tree

Every seat works in its own worktree, `.claude/worktrees/235-<seat>/`,
detached at the commit of `235-base` that holds these briefs (its first
parent is the lane `pm-opening`, its second `core-b1`, the first batch of the
core packages, so the tree holds the 14 programs that carry `core/`). The
probe was run at `9d894fe68` (parents `6656268fe` and `9791d4787`, tree
`f01b6059aeb1f0f2d86c085fecbdb7eb0a688f43`); `235-base` gains the briefs on
top of it, and the seat checks its own `git log -1`. **Build your own compiler
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
- `use <json>/parse` is refused at the `<` (`expected_module_name`);
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
  name.
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
2. **The spelling**: how a `use` line marks a fetched dependency (the author's
   `<…>` and its competitors), and every plausible mistake (the mark
   forgotten or added, the domain as Go writes it, `import`) as an error with
   a `certain` fix.
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
