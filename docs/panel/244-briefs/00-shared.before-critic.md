# Panel 244, the brief every seat reads first

Convened 2026-11-10 by the coordinator of batch 30, in M-core-packages, on the author's
instruction of that day, meant as: *document packages well on the site and in the guides, with
the reasons behind each choice, and put what is missing into the specification*. The guide is a
lane's (`b30-docs`); this sitting rules on **the specification's half**, a change to the language's
text (CLAUDE.md § 4). Seats: spec-warden and compiler-engineer, and the completeness critic before
and after. The llm-ergonomist's blind reading is a paid run and is **not** named here; the
coordinator asks the author for it, and the synthesis records whether it ran.

## The base

`main` at `89db84347`, its stage 2 compiler `sources 14e9e5ce168a2336`. Each seat with Bash works
in its own detached worktree `.claude/worktrees/244-<seat>` at that commit, builds its own compiler
from the seed in two stages, probes in `.claude/worktrees/p244-scratch/<seat>/`. One heavy command
at a time. No paid run (no `measure --refresh`, no `claude -p`), no credential, no GitHub write,
no push, no file outside the repository's root. Reports to `docs/panel/244-reports/<seat>.md` in
`.claude/worktrees/p244`, or, if that write is refused, your final message is the report.

## What the specification says today, measured

Spec § 1 (`spec/heroes-spec.md:21` on, `grep -n '^## 1\.'`) holds the package text panel 235's R13
chose, the short draft with its catalogue example (`docs/panel/235-*.md:491-494`: the critic
refused to drop the example, 236 of 240 readers having copied it). Counted on 2026-11-10 with
`grep -c -i <word> spec/heroes-spec.md`: `internal` 0, `heroes.packages` 0, `path =` 0, `heroes
fetch` 0, `vendor` 0, `private_module` 0, `fingerprint` 1, `heroes add` 1. The spec reads
`real 10898` tokens (`heroes measure spec/heroes-spec.md`, pinned 2026-11-09) against the ceiling
of 16,384 (`tests/harness/suite_spec.hero:162`).

## What the compiler holds that the specification does not say (each to be run by the seats)

- **`internal/`**, panel 235's R16 (`docs/panel/235-*.md:758`): a module under an `internal`
  folder belongs to the module before the last `internal` part, and only it and the modules under
  it name it; `private_module` otherwise (`selfhost/module/package_told.hero:259`,
  `selfhost/placed_use.hero` `internal_owner`); it binds the program's own `internal/` folders too.
  R16 landed after R13's text; no record found by the coordinator says it was kept out of the spec
  on purpose (searched: `grep -n -i 'spec' docs/panel/235-*.md` for `internal`, `R16`), a question
  rather than a premise.
- **Where fetched code stands**: `heroes fetch` places every package the catalogue names under
  `heroes.packages/` beside the catalogue, each checked against its sum; `build`, `check` and `run`
  never fetch (`selfhost/cli/table.hero:184`'s summary); a placed tree is held to its `sum` at build
  (lane b29-envfetch's report, unrun by the coordinator: run it).
- **The sources of an entry**: an index name, a `git` address with a tag and commit, a folder
  (`path`), `heroes add`'s three forms (`table.hero:183`); spec § 1's example shows the first two.
- **`use <entry>` alone** (`package_wants_a_module`), and a `use` naming a module the package has
  not (`unknown_package_module`).

## The question

Which of these the specification states, in what words, at what cost, and which it leaves to the
compiler's messages, by Principle 0 (CLAUDE.md § 2) and design.md §1.6's budget: a sentence enters
where a reader holding only the specification would otherwise write a wrong program or be unable
to predict a refusal, measured, never because it is true. For each candidate: the sentence, its
vendored token count, the plausible mistake it prevents and the error the compiler gives that
mistake today (run), and whether that error alone already tells the reader what the sentence would.
