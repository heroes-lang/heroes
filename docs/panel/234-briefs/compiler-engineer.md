# Panel 234, the compiler-engineer's brief

Read `00-shared.md` first, then `proposal.md`, `conventions-draft.md` and
`probe/README.md`. You judge the ceiling (design.md §1.1, §1.7, Part 5):
what each route costs the compiler and the runtime, in files and lines you
cite, and whether it is core or sugar. You have a veto on soundness.

**Your worktree** is `.claude/worktrees/234-compiler-engineer/`, detached at
the commit that adds these briefs; build your compiler there from the seed
and check its key (`00-shared.md`, *The tree*). Your scratch is
`.claude/worktrees/234-compiler-engineer-scratch/`. Your report is
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/234-reports/compiler-engineer.md`,
written as you go, one section per item. Every number in it is a command you
ran, named beside it; a negative sentence is a command you ran, or it is
written as a question.

## Your items

**B, where packages live** (the heart of your seat here). For each route, B1
to B6 and any you find (the shared brief lists them; B5, the tree compiled
into the compiler, is panel 028 R3's P-embed applied to packages, and owes a
price: the compiler's size, a release to change a package, panel 097's
condition 5 read against it, and what `heroes fetch` would fetch; B6, a
symbolic link per program, owes what a Windows checkout does with it, unrun):
what the compiler already does (probes 1 and 2 are
yours to re-run in your worktree), what it would have to change and where
(`selfhost/modules.hero`, `selfhost/module/names.hero:66` `module_of`, the C
component panel 099 R3 makes of a path, the cache under `build/`), and what
each does to the C names a program's modules get (the critic measured
`h_corestrings_contains` beside `h_strings_contains`, and
`module_names_collide` for a `corestrings.hero` beside `core/strings`, F5;
extend it). Price the root driver against `heroes test` per package, knowing
that a driver must name something from each package it `use`s and that
`heroes test` reaches modules transitively (F31). Say what **the nine suites
that name `examples`** (the shared brief's item B, F12) do with a tree of
packages placed under `examples/` and with a copy of it inside a program.
**And price the header lookup** (F15): a group's header is found from the
compiled file's directory, so a package shipping its own header builds from
the root only with a placement-bound spelling or `--include`; price the
route nobody listed until the critic, the compiler searching the declaring
module's directory first for its groups' headers, as C does for a quoted
`#include`, against leaving it, and say which of B1 to B6 each serves. Read
`docs/roadmap/milestones/M-package-manager.md` and say which of B1 to B4
`heroes fetch` can later serve without changing what a program's `use` lines
say.

**C, what the compiler enforces of the conventions and what only a check
could.** Go through `conventions-draft.md` rule by rule and mark each
*enforced by the compiler* (with the diagnostic code, as probes 3 to 5 do),
*checkable by a check of the net* (say which suite would hold it), or
*convention only*. In particular: §1.3 (private modules nobody `use`s), §1.5
(names differing in case: does the compiler refuse two modules whose paths
differ only in case, or a `use` spelled in another case than the file on a
case-insensitive file system; run it on this Mac), §2.3 (a module taking a
built-in's name: is `use sort` beside the built-in `sort` a hazard a reader
can trip on, and should it be refused), and the near-name suggestion of probe
4: read `selfhost/resolve/near_names.hero` and price a suggestion for Go's
`index` against `index_of` (a diagnostic change, which would need its own
sitting; price it, do not land it).

**E, the card.** Price the three routes of the shared brief's item E: a card
made by the compiler (a flag on `check`, or `heroes doc`; read
`selfhost/cli/table.hero`, `selfhost/cli/flags.hero`, CLAUDE.md § 10 and
`.claude/rules/cli-surface.md`; `--dump-scopes` gives names alone, F32), a
card written by hand and held true by a check of the net, and a program of the
harness that writes or checks each card from its module (F32). Which can tell a
card that has drifted, and which § 10 admits.

**F, the platform layer, its cost in the compiler and the runtime.** Read how
the runtime is found, keyed, compiled and linked (`selfhost/cli/runtime_key.hero`,
`selfhost/cli/toolchain.hero`, `runtime/runtime.c`, `HERO_RUNTIME_ABI` in
`runtime/heroes_runtime.h`) and price **both axes** of the shared brief's item
F (F36): on the object axis, P2, a second object beside the runtime, its own
key and assertion, linked always or only where a group names its header (what
does the compiler know, at link time, about the headers a program's groups
name); on the header axis, P1', the runtime's one object behind a family of
headers, and what the seeds (`SEEDS`, every unit sees `hero_os.h`) do to a
family. **And price panel 114 R3's second instrument** (F7): the compiler
reporting which `extern` declarations it did not verify on this machine, which
the ffi seat's veto makes owed by every route with two arms in C; say what it
would take and where. Price P1 in the other direction: what the
runtime's one object grows by if sockets, clocks and the rest join it, and
whether the linker drops what a program does not call (measure with `nm` on a
small program's binary today: are the runtime's unused functions in it).
Say whether P3 (a header of `static inline` functions a package ships) needs
any compiler change (the shared brief's seven `fixedbugs` headers are the
evidence it does not; confirm) and what it does to the build cache.

**I, a group's header.** The critic ran it: the seeds alone explain
`selfhost/cli/process.hero:66-69` (F9). Price the two checks the three
positions imply, per group without the seeds and per group with them: what
each would take (`selfhost/emit/ffi_declared.hero`,
`selfhost/emit/externs.hero`), and what each would refuse in the tree today
(count the groups each would refuse, by a command).

**K, the repairs.** Confirm the shared brief's six findings against the
tree, each with its own command (the dates of `exit`, `args_checked` and
`validated` corrected by the critic, F28), and say for the `#include` sentence
whether design.md:2583's paragraph names the seeds (`SEEDS`, `heroes_unit.h`
and `hero_os.h`).

**M, `sort_by`.** Two ratified rulings meet here (F27): panel 097's
condition 5 and panel 137's adoption. Re-verify issue 1154's four hand-written
sorts in `selfhost/` (the lines it gives are of 2026-09-13), count their lines
today, and price the three homes: the built-in library
(`selfhost/library_source.hero`), a `sort` package (which the compiler cannot
`use`, by probe 1's rule, unless route B5 holds: say what it would take), or
neither. Panel 137's prediction was *at most 20 lines added, at least 60
deleted, no compiler module changed*.

**N, the instrument.** Read `tests/harness/thesis_sandbox.hero` and
`tests/harness/thesis_grade.hero` and say whether a task can carry modules
beside its answer today (a `core/` tree the answer `use`s), and if not, what
it would take; this is what the thesis harness's before-and-after run of
question (xiii) needs.

**A, the demand.** Re-run issue 805's count and extend it: which of
`tests/harness/strings.hero`'s 15 functions have a copy in `examples/`, and
how many lines each copy is (the substitution Goose's test measures).

## Report shape

Per item: **verdict** (approve, object, or veto, and on which route), **section**
(design.md or spec), **cost** (files and lines, cited), **prediction** (a
falsifiable sentence and the milestone at which it is checkable),
**condition** (what would change your verdict). Then a short list of what you
could not run and why.
