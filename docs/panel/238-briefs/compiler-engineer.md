# Panel 238, the compiler-engineer's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/238-compiler-engineer/`, detached at the briefs' commit; build your compiler
there in two stages as `00-shared.md` says. You judge implementation cost and core against sugar
(design.md §1.1, §1.7, Part 5) with veto power; cite files and line counts (`wc -l` at the
briefs' commit). This is a `retro-record` sitting: the author decided; your job is the strongest
measured objection, the details the author left open settled by measurement, and the cost. The
ffi-pragmatist sits beside you on the C side of the order (F20, the link line, static archives on
Linux); the C you need for your own items you run yourself, and you do not duplicate its work.

## Where the formatter lives (`wc -l` at `bd4670da4`)

- `selfhost/print/fmt.hero` (1827): `format_lexed` (from line 125) builds the `Item` list of uses
  and declarations, sorts it by source offset (`sorted_items`, line 302), prints a `use` with
  `use_line` (line 338, the alias kept), and rules the blank lines between top-level items (lines
  217-247: none between two `use` lines; a blank between a header comment and the first `use`
  kept, lines 234-237).
- `selfhost/print/page.hero` (474) and `selfhost/print/owners.hero` (389): the comment stream and
  each comment's owner, which a reorder must carry with its line.
- `selfhost/print/guard.hero` (264), `selfhost/print/anchors.hero` (460) and
  `selfhost/print/dump.hero` (263): the four checks of `fmt`'s output (F8); `checked` at lines
  71-79 refuses on the parser's diagnostics alone (F4).
- `selfhost/probe/reader.hero`: `heroes probe`'s independent reader (panel 179).
- `selfhost/cli/syntax_cmds.hero`: the formatter's self-check (its tests at lines 227-228 and
  284-285 pair a `use` with and without its alias and brackets).
- `selfhost/parse/use_line.hero:273` (the `certain` fix `check --apply` writes into a `use` line)
  and `selfhost/resolve/missing_use.hero:66` (a `guess` that inserts one at offset 0) (F9).
- The module walk, the resolver and the units: `selfhost/module/` (15 modules),
  `selfhost/resolve/` (`walk.hero`, `top.hero`, `cycles.hero`, `unused_sweep.hero`) and
  `selfhost/cli/one_file.hero`, where the written order reaches the diagnostics' order, the holes'
  order and the emitted C (F19 to F21).

## What the sitting needs from you, each measured

1. **Question A, the rule.** Prototype it in your worktree, the most robust form you can defend,
   and say for each detail why: the key among the routes `00-shared.md` § A lists (the whole line by
   bytes, part by part, case-folded, the bound name; F7 says which characters each part holds), the
   blank line between the groups or not, a comment above, beside and between `use` lines (F6, F8,
   F17: design.md §4.1's rule as the default, `selfhost/emit.hero`'s header the first case), runs
   separated by blank lines or comments, a `use` after a declaration (F3; `response_file.hero`'s two
   under `## Tests`), and the alias. The canonical form is *exactly one correct way to write any
   program* (design.md §4.15, F17): say whether each choice keeps that true, that is, whether two
   files holding the same `use` lines in different orders and layouts format to one text.
2. **Question A, the premise, and the route that would make it true.** F4, F19, F20, F21 and F22
   measure the written order moving the checker's verdict and fix, the diagnostics' and holes' order,
   the one-file emission and whether `--emit-c` accepts a program. Re-run them in your worktree, add
   `selfhost/main.hero` reordered by hand (its `--emit-c` with `hero_program_sources` out, and the
   build cache's keys under `build/`), and each `--dump-*` printer. Then **price the route that makes
   the premise true in the compiler**: every place the written order of `use` lines reaches an
   output (the walk, the units, the one-file emission's order, the link words, the diagnostics' sort,
   the holes' list), what key each would take instead (the sorted order the formatter writes, so
   that a formatted file and its source mean the same by construction), the lines it costs by
   module, and what it moves (every blessed emission whose modules are not already in that order,
   counted). Say whether the formatter rule may land without it, given that a reorder by `fmt` then
   changes outputs the author called immaterial, and which is the robust choice.
3. **Question B, the cost, with your prototype.** Run your `heroes-next fmt` over every tracked
   `.hero` outside `docs/` and count, by directory, the files it changes (replacing F5);
   idempotence (`fmt` twice); the guard (F8) over every changed file, with no exit 2; which goldens
   see a line move (`.expected`, `.fixed`, `.applied`, a `run` case's output naming a `file:line`),
   which of them `canonical` walks (F10) and which it exempts; **the three goldens whose subject is
   the order (F22)**: run the compiler at the commit before defect 560's repair (`git log -S
   fixedbugs560` gives the repair, `1062f34f1`, 2026-10-10; build its parent in a scratch worktree
   under your `build/`) and before defect 563's (`git log --diff-filter=A` on its case gives
   `1eed23d94` and `1ec192bd8`, the same day) on both orders of each case, and say whether a sorted
   case still catches its defect, and the route (exempt it as `check/` is exempt, rename its modules
   so the pinned order also sorts first, or another). Then reformat `selfhost/`, `examples/` and
   `tests/golden/run/` with your prototype, rebuild in two stages, and say what moves: the compiler's
   emitted C beyond `hero_program_sources`, the fixpoint (stage 2 against stage 3 by `cmp`), the
   compiler's own tests, and `tests/emission/**` and `tests/golden/emit/**` (F21: `examples/shapes`
   alone moves 50 diff lines), counted. Run, narrowed where a whole form costs too much: `heroes test
   selfhost/main.hero`, and the harness's `canonical`, `probe`, `surface`, `fixes`, `annotations`,
   `emission` and `run` over what your reformat touched. Count the prototype's lines by module. Say
   what `heroes mutate` and the probe's reader need, and whether `check --apply` must write a sorted
   block (F9: no `fmt` call in `--apply` today) or the rule accepts an `--apply` output `fmt` would
   change. Count the lanes a reformat would meet by F23's command (three dots from the merge base,
   over `git branch --no-merged bd4670da4`), never touching a lane.
4. **Question B, the landing order.** The hooks refuse a write of a non-canonical file (F10) and the
   site's start page says the tree is canonical byte for byte (F16), so propose the order (the rule,
   its tests, the compiler route of item 2 if adopted, the tree reformatted, the goldens and
   emissions it moves, the seed, in which commits, and which of them publish the site) and what a
   lane opened before the landing does at its next merge.
5. **Question C, where the rule is written.** Draft the text for design.md §4.15 and for
   `fmt.hero`'s policy comment (F2), the help line (`selfhost/cli/table.hero:182`), and say which
   site pages describe `fmt` (`git grep -n 'heroes fmt' -- site/src`) and whether any moves. Hand
   the spec-warden nothing: it reads the specification itself.
6. **Question D, the index.** In your worktree: move `examples/index/` to `packages/` at the root,
   rewrite the 14 catalogues' `index` line to `file://../../packages`, and run `heroes fetch` (and
   `heroes add` into a scratch program) for at least two programs; say whether F14's rule accepts it
   and what F13 renames. Run `.claude/rules/verification.md` § The map's command over the moved tree
   and the suites it names for `packages/` and `examples/` (each narrowed where possible, `heroes run
   tests/harness/main.hero -- ./heroes-next <suite>`), `probe`, `compat`, `canonical`, `records`,
   `unseen`, `packages` and `fetch` among them. Run `npm ci` and `npm run build` in your worktree's
   `site/` (free; no paid run) with `examples-model.ts`'s `INDEX` changed, or say why not. Price the
   routes of `00-shared.md` § D: a placed key that does not hold the index address (read
   `placed.hero:74-77` and panel 235's R7 for why it does; say what the falsifier loses), the other
   readings of the third instruction, and the name against `vendor/` at the root.
7. **The shapes beside it, depth one.** A file of only `use` lines; a `use` with a trailing comment
   (only `#~` marks carry one today, F5); a `use` whose alias would sort it elsewhere than its path;
   a package's own module (inside `heroes.packages/`), whose bracketed `use` lines name its own
   catalogue; two groups written in the wrong order with a comment between; a late `use` between a
   comment and a declaration (F8). A real defect of another cause goes in your report with its
   reproducer.

Your report: `docs/panel/238-reports/compiler-engineer.md` in your worktree, written as you go,
ending as `00-shared.md` § Rules says, in the structure of `.claude/agents/compiler-engineer.md`.
