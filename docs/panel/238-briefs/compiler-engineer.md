# Panel 238, the compiler-engineer's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/238-compiler-engineer/`, detached at the briefs' commit; build your compiler
there in two stages as `00-shared.md` says. You judge implementation cost and core against sugar
(design.md §1.1, §1.7, Part 5) with veto power; cite files and line counts (`wc -l` at the
briefs' commit). This is a `retro-record` sitting: the author decided; your job is the strongest
measured objection, the details the author left open settled by measurement, and the cost.

## Where the formatter lives (`wc -l` at `bd4670da4`)

- `selfhost/print/fmt.hero` (1827): `format_lexed` (from line 125) builds the `Item` list of uses
  and declarations, sorts it by source offset (`sorted_items`, line 302), prints a `use` with
  `use_line` (line 338, the alias kept), and rules the blank lines between top-level items (lines
  217-247: none between two `use` lines; a blank between a header comment and the first `use`
  kept).
- `selfhost/print/page.hero` (474) and `selfhost/print/owners.hero` (389): the comment stream and
  each comment's owner, which a reorder must carry with its line.
- `selfhost/print/guard.hero` (264), `selfhost/print/anchors.hero` (460) and
  `selfhost/print/dump.hero` (263): the four checks of `fmt`'s output (F8).
- `selfhost/probe/reader.hero`: `heroes probe`'s independent reader (panel 179).
- `selfhost/cli/syntax_cmds.hero`: the formatter's self-check (tests at lines 227-228 and
  284-285 already pair a `use` with and without its alias and brackets).
- `selfhost/resolve/missing_use.hero` (the `Fix` at line 66, F9).
- The module walk and the resolver: `selfhost/module/` (15 modules) and `selfhost/resolve/`
  (`walk.hero`, `top.hero`, `cycles.hero`, `unused_sweep.hero`), where an order of `use` lines
  could reach a diagnostic's order or the order modules are emitted.

## What the sitting needs from you, each measured

1. **Question A, the rule.** Prototype it in your worktree, the most robust form you can defend,
   and say for each detail why: the sort key (F7: the whole line or the path part by part; what
   characters a module part and a package name may hold, `heroes add Json` and a module file named
   with an uppercase letter or a digit tried), the blank line between the groups or not, a comment
   above, beside and between `use` lines (F6's `selfhost/emit.hero` header), runs separated by blank
   lines or comments, a `use` after a declaration (F3; `selfhost/cli/response_file.hero:144-145` keeps
   two under `## Tests` on purpose), and the alias. The canonical form is *exactly one correct way to
   write any program* (design.md §4.15, F17): say whether each choice keeps that true, that is,
   whether two files holding the same `use` lines in different orders and layouts format to one
   text.
2. **Question A, the premise.** Reorder by hand the `use` lines of real programs (at least one
   `examples/` program with both groups from F5's list, one with an alias, one `tests/golden/run`
   case with several modules, and `selfhost/main.hero`) and compare, before and after: the program's
   run output; `heroes check` and `heroes check --json` (diagnostics' order and text, the holes
   list), with a planted mistake in two modules so that more than one module reports; `--emit-c` with
   the line `hero_program_sources` taken out of each (`grep -v 'hero_program_sources(void) {'`);
   each `--dump-*` printer; and the build cache's keys (`build/`). An order that moves anything is
   a finding the author's premise did not hold, with its reproducer.
3. **Question B, the cost, with your prototype.** Run `heroes-next fmt` (yours) over every tracked
   `.hero` outside `docs/` and count, by directory, the files it changes (replacing the census of
   F5); idempotence (`fmt` twice); the guard (F8) over every changed file, with no exit 2; which
   goldens see a line move (`.expected`, `.fixed`, `.applied`, a `run` case's output naming a
   `file:line`), which of them `canonical` walks (F10) and which it exempts; `tests/golden/surface-
   fixtures` and the `probe`, `surface` and `fixes` suites that read them. Then reformat `selfhost/`
   with your prototype, rebuild in two stages, and say what moves: the compiler's emitted C beyond
   `hero_program_sources`, the fixpoint (stage 2 against stage 3 by `cmp`), the compiler's own tests,
   `tests/emission/**` and `tests/golden/emit/**`. Run, narrowed where a whole form costs too much:
   `heroes test selfhost/main.hero`, and the harness's `canonical`, `probe`, `surface`, `fixes`,
   `annotations`, `emission` and `run` over the cases your reformat touched. Count the prototype's
   lines by module. Say what `heroes mutate` and the probe's reader need, and what the `Fix` of F9
   must write so that `check --apply`'s output is canonical. And how many open lanes'
   branches the reformat would meet in a merge (`git worktree list`, then `git diff --stat` of each
   lane branch against `bd4670da4` restricted to `selfhost/`), never touching a lane.
4. **Question B, the landing order.** The hooks refuse a write of a non-canonical file (F10), so
   propose the order (the rule, its tests, the tree reformatted, the goldens it moves, the seed, in
   which commits) and what a lane opened before the landing does at its next merge.
5. **Question D, the index.** In your worktree: move `examples/index/` to `packages/` at the root,
   rewrite the 14 catalogues' `index` line to `file://../../packages`, and run `heroes fetch` (and
   `heroes add` into a scratch program) for at least two programs; say whether F14's rule accepts
   it, what F13 renames, and what the `packages`, `corpus`, `records`, `unseen`, `canonical` and
   `fetch` suites do over the moved tree (each narrowed where possible, `heroes run
   tests/harness/main.hero -- ./heroes-next <suite>`). Read `site/src/lib/examples-model.ts` and
   say what the site's build would need (run `npm run build` in `site/` only if `site/node_modules`
   exists in your worktree; otherwise say it is unrun and owed by the landing). Price the other
   reading (`proposal.md`): what a catalogue with no `index` line does today (panel 235's R5, the
   Heroes index by default) and what changing that default to a project-root `packages/` would
   touch. Say whether `packages/` collides with anything (F18's names) and what would make it wrong.
6. **The shapes beside it, depth one.** A file of only `use` lines; a `use` line with a trailing
   comment; a `use` line whose alias would sort it elsewhere than its path; a package's own module
   (inside `heroes.packages/`), whose bracketed `use` lines name its own catalogue; a file whose two
   groups are written in the wrong order with a comment between. A real defect of another cause
   goes in your report with its reproducer.

Your report: `docs/panel/238-reports/compiler-engineer.md` in your worktree, written as you go,
ending as `00-shared.md` § Rules says, in the structure of `.claude/agents/compiler-engineer.md`.
