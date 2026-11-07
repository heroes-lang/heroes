# Panel 235, the compiler-engineer's brief

Read `00-shared.md` first, then `proposal.md`, `probe/README.md` and the
survey's Table 1 and Table 2 (Go's and Cargo's rows). You judge the ceiling
(design.md §1.1, §1.7, Part 5): what each route costs the compiler and the
runtime, in files and lines you cite, and whether it is core or sugar. You
have a veto on soundness.

**Your worktree** is `.claude/worktrees/235-compiler-engineer/`, your scratch
`.claude/worktrees/235-compiler-engineer-scratch/`, your report
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/pm-opening/docs/panel/235-reports/compiler-engineer.md`.
Building an edited compiler in your worktree is allowed and expected: stage 2
from the seed's compiler is about a minute (`/panel` § 2).

## Your questions

**0, the base, from the compiler's side.** What Go's minimal version
selection costs to write in Heroes against Cargo's solver (the survey's 2.1
and 2.2, probe 4); whether a lock file is needed under each; and what each
asks of the catalogue and of a fingerprint file.

**2 and 3, the spelling and the resolution.** For each spelling in
`blind/README.md` (U-angle, U-bare, U-pkg), the lexer, parser and resolver
change it needs, cited by file and line: `Use` in spec § 1,
`selfhost/parse/use_line.hero`, the second reader of module paths in
`selfhost/module/paths.hero`, and `selfhost/modules.hero:107`, where every
`use` is read from the root's directory. Then: a dependency's own `use` lines
starting from the dependency's root and not the consumer's; the C names its
modules get (`selfhost/module/names.hero`, the whole path sanitised to
`[A-Za-z0-9]`), and whether two fetched packages or a package and a program
folder can collide there; and each plausible mistake of a spelling as a
diagnostic with a `certain` fix, written as the diagnostics rules require.

**4, 5 and 7, the catalogue, the fingerprint, where code lives.** The parser
each format needs, in lines of Heroes (a TOML subset, a YAML subset with its
scalar rules, a line format, a declaration in Heroes' own syntax that the
existing parser could read); the fingerprint over a fetched tree's bytes when
`sha256.of` takes a `str` and a file may not be text; and, for a tree placed
inside the program against a cache, what `build/`'s per-module cache, the
`corpus`, `packages`, `records` and `unseen` suites and `compat` must change.

**12 and 14, the verbs and the interface.** Where a verb enters
(`selfhost/cli/table.hero`, its pinned count of 13, the arms in
`selfhost/main.hero`, `site/src/lib/claims.ts`), the verbs each base implies,
and the interface that must stay fixed from the first version (over C) to the
second (over the core packages), as goldens.

**The latency.** The rule *never slow the compiler down*: what reading a
catalogue costs a build that has none, and a build that has one.

## What you prototype

The cheapest robust route far enough that the number is real: a compiler that
reads one catalogue entry and resolves one fetched `use` from a placed tree,
in your worktree, its cost in lines and in the `check` and `build` time of
`selfhost/main.hero` before and after (`/usr/bin/time -p`, the best of N runs,
the first discarded; say the machine was not still, since other seats run).
A route that does not build is not adopted (`/panel` § 3c).

## And from the critic's first pass

The questions the shared brief's table gives you beyond those above
(`00-shared.md` § The sitting's questions):

- **16, alias scope**: a dependency's own `use <x>/…` lines resolved by whose
  catalogue; two catalogues giving one alias to two packages, or two aliases
  to one, under one version per program; whether an index package may be
  named otherwise than its index name, and what the C names
  (`selfhost/module/names.hero`) give each case.
- **20, the catalogue's grammar in productions**, for each format of the
  reader test's F arms (TOML, YAML, JSON, the line format, Heroes' own
  syntax), beside its parser's lines of Heroes.
- **9**, a program with no catalogue compiling as today, held by the 69
  programs of `v0.6.0` that `compat` builds.
- **11 and 12**: `core` as a catalogue entry against the 14 copies of `core/`
  in `235-base`, which carry 19 modules under `internal/` (probe); what an
  `internal/` folder means inside a fetched package, and what the `packages`
  suite becomes.
